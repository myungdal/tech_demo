// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Serializer/Serializer.h"
#include "Common/Serializer/Deserializer.h"
#include "Common/UsageMeter/UsageMeter.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 압축 메모리 풀 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 캐싱 적중률에 중점을 둔 메모리 풀 (연속 메모리 사용).
// - 책임: 키 기반 객체 할당/해제 관리 (연속 메모리 레이아웃)
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template<typename _DataType, typename _KeyType = int16_t, size_t DefaultCapacity = 16>
class CompactionPool final
{
public:
	using KeyType = _KeyType;
	using KeyView = std::vector<KeyType, MallocAllocator<KeyType>>;

	static constexpr KeyType INVALID_KEY = static_cast<KeyType>(-1);  // 무효 키 값

	using DataType = _DataType;
	using DataList = std::vector<DataType, MallocAllocator<DataType>>;

private:
	DataList mDataList;              // 데이터 배열 (연속 메모리)
	KeyView mKeyView;                 // 키 뷰 배열 (활성화 여부)
	KeyType mActiveDataCount = 0;     // 활성 데이터 개수

public:
	CompactionPool()
	{
		static_assert(std::is_trivially_copyable_v<DataType>);

		UsageMeter<CompactionPool, DataType>::OnInit(DefaultCapacity);

		ReservePool();
	}

	virtual ~CompactionPool()
	{
		Clear();
	}

public:
	// 모든 데이터를 제거한다.
	void Clear()
	{
		// 살아 있는 객체 소멸
		for (KeyType i = 0; i < static_cast<KeyType>(mDataList.size()); ++i)
		{
			if (mKeyView.at(i) != INVALID_KEY)
			{
				DataType* ptr = std::launder(reinterpret_cast<DataType*>(&mDataList[i]));
				ptr->~DataType();
				mKeyView.at(i) = INVALID_KEY;
			}
		}

		UsageMeter<CompactionPool, DataType>::OnDecrease(mActiveDataCount);

		mActiveDataCount = 0;
	}

	// 풀을 직렬화한다.
	// - serializer: 직렬화기 참조
	void Serialize(Serializer& serializer)
	{
		KeyType poolSize = static_cast<KeyType>(mDataList.size());
		serializer.WriteValue(poolSize);
		serializer.WriteValue(mActiveDataCount);		
		
		serializer.WriteBinary(reinterpret_cast<uint8_t*>(mDataList.data()), sizeof(DataType) * poolSize);
		serializer.WriteBinary(reinterpret_cast<uint8_t*>(mKeyView.data()), sizeof(KeyType) * poolSize);
	}

	// 풀을 역직렬화한다.
	// - deserializer: 역직렬화기 참조
	void Deserialize(Deserializer& deserializer)
	{
		KeyType poolSize = deserializer.ReadValue<KeyType>();
		mActiveDataCount = deserializer.ReadValue<KeyType>();

		mDataList.resize(poolSize);
		mKeyView.resize(poolSize);

		deserializer.ReadBinary(reinterpret_cast<uint8_t*>(mDataList.data()), sizeof(DataType) * poolSize);
		deserializer.ReadBinary(reinterpret_cast<uint8_t*>(mKeyView.data()), sizeof(KeyType) * poolSize);

		UsageMeter<CompactionPool, DataType>::OnReset();
		UsageMeter<CompactionPool, DataType>::OnChangeLimitation(poolSize);
		UsageMeter<CompactionPool, DataType>::OnIncrease(mActiveDataCount);
	}

public:
	// 객체를 할당하고 키를 반환한다. 풀이 가득 차면 확장.
	// - return: 할당된 키. 실패 시 INVALID_KEY
	KeyType Pop()
	{
		if (true == NeedsReservePool())
			ReservePool();

		++mActiveDataCount;

		KeyType key = 0;
		while (mKeyView.at(key) != INVALID_KEY)
			++key;

		mKeyView.at(key) = key;

		// 생성자 호출
		DataType* data = reinterpret_cast<DataType*>(&mDataList.at(key));
		::new (data)DataType();

		UsageMeter<CompactionPool, DataType>::OnIncrease();

		return key;
	}

	// 키에 해당하는 객체를 해제한다.
	// - key: 해제할 키
	void Push(KeyType key)
	{
		if (key == INVALID_KEY)
			return;

		mKeyView.at(key) = INVALID_KEY;
		--mActiveDataCount;

		// 소멸자 호출
		DataType& data = *std::launder(reinterpret_cast<DataType*>(&mDataList.at(key)));
		data.~DataType();

		UsageMeter<CompactionPool, DataType>::OnDecrease();
	}

	// 키에 데이터가 있는지 확인한다.
	// - key: 확인할 키
	// - return: 데이터가 있으면 true
	bool HasData(KeyType key) const
	{
		if (key == INVALID_KEY)
			return false;

		if (key < 0 || key >= static_cast<KeyType>(mKeyView.size()))
			return false;

		if (mKeyView.at(key) == INVALID_KEY)
			return false;

		return true;
	}

	// 키에 해당하는 데이터를 반환한다.
	// - key: 데이터 키
	// - return: 데이터 참조
	DataType& GetData(KeyType key)
	{
		return *std::launder(reinterpret_cast<DataType*>(&mDataList.at(key)));
	}

	// 키에 해당하는 데이터를 반환한다 (const).
	// - key: 데이터 키
	// - return: 데이터 참조
	const DataType& GetData(KeyType key) const
	{
		return *std::launder(reinterpret_cast<const DataType*>(&mDataList.at(key)));
	}

	// 데이터 리스트를 반환한다.
	// - return: 데이터 리스트 참조
	const DataList& GetDataList() const noexcept
	{
		return mDataList;
	}

	// 활성 데이터 개수를 반환한다.
	// - return: 활성 데이터 개수
	KeyType GetActiveDataCount() const noexcept
	{
		return mActiveDataCount;
	}

	// 풀 크기를 반환한다.
	// - return: 풀 크기
	KeyType GetPoolSize() const noexcept
	{
		KeyType poolSize = static_cast<KeyType>(mDataList.size());
		return poolSize;
	}
private:
	bool NeedsReservePool() const
	{
		if (GetPoolSize() <= mActiveDataCount)
			return true;

		return false;
	}

	void ReservePool()
	{
		const size_t oldSize = mDataList.size();

		size_t newSize = oldSize + (oldSize >> 1) + 1;
		newSize = std::max(newSize, DefaultCapacity);

		mDataList.resize(newSize);
		mKeyView.resize(newSize, INVALID_KEY);

		UsageMeter<CompactionPool, DataType>::OnChangeLimitation(newSize);
	}
};
