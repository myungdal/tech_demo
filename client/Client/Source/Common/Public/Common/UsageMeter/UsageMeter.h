// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 사용량 측정 유틸리티 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 클래스의 사용량을 측정하는 데이터 구조.
// - 책임: 증가/감소/제한값 추적 및 타입 이름 저장
struct UsageMeterData
{
	std::atomic<int64_t> mIncrease = 0;      // 증가 횟수
	std::atomic<int64_t> mDecrease = 0;      // 감소 횟수
	std::atomic<int64_t> mLimitation = 0;     // 제한값

	std::string mParentNameA = "";           // 부모 타입 이름 (UTF-8)
	std::wstring mParentNameW = L"";         // 부모 타입 이름 (UTF-16)

	std::string mClassNameA = "";            // 클래스 타입 이름 (UTF-8)
	std::wstring mClassNameW = L"";          // 클래스 타입 이름 (UTF-16)
};

// 전역 맵 - Meyer's Singleton 패턴 (Static Initialization Order Fiasco 방지).
using UsageMeterDataMap = std::map<std::pair<std::string, std::string>, UsageMeterData*>;

// 사용량 측정 데이터 맵을 반환한다.
// - return: 데이터 맵 참조
inline UsageMeterDataMap& GetUsageMeterDataMap()
{
	static UsageMeterDataMap instance;
	return instance;
}

// 사용량 측정 데이터 맵 뮤텍스를 반환한다.
// - return: 뮤텍스 참조
inline std::mutex& GetUsageMeterDataMapMutex()
{
	static std::mutex instance;
	return instance;
}


// 사용량 측정 템플릿 클래스.
// - 책임: 타입별 사용량 추적 및 통계 관리
template <typename _Parent, typename _Class>
class UsageMeter
{
private:
	// 전역 맵에서 데이터를 가져오거나 생성한다.
	static UsageMeterData& GetData()
	{
		// [WHY] 타입 이름을 키로 사용 (string 기반으로 변경)
		static std::string parentName = typeid(_Parent).name();
		static std::string className = typeid(_Class).name();
		static auto key = std::make_pair(parentName, className);

		std::lock_guard<std::mutex> lock(GetUsageMeterDataMapMutex());

		auto& dataMap = GetUsageMeterDataMap();
		auto it = dataMap.find(key);
		if (it != dataMap.end())
		{
			return *it->second;
		}

		// [WHY] 새로 생성하여 맵에 추가
		UsageMeterData* newData = new UsageMeterData{};
		newData->mParentNameA = parentName;
		newData->mParentNameW = SimplifyTypeName(ConvertToWstring(parentName));
		newData->mClassNameA = className;
		newData->mClassNameW = SimplifyTypeName(ConvertToWstring(className));

		dataMap.emplace(key, newData);
		return *newData;
	}

private:
	// 타입 이름을 단순화한다 (class/struct 접두사 제거).
	// - fullName: 전체 타입 이름
	// - return: 단순화된 이름
	static std::wstring SimplifyTypeName(const std::wstring& fullName) 
	{
		std::wregex templateRegex(LR"(class\s+|struct\s+|<.*>)");
		return std::regex_replace(fullName, templateRegex, L"");
	}

	// UTF-8 문자열을 UTF-16으로 변환한다.
	// - strA: UTF-8 문자열
	// - return: UTF-16 문자열
	static std::wstring ConvertToWstring(const std::string& strA)
	{
		std::wstring strW{ L"" };

		size_t requiredSize = 0;
		mbstowcs_s(&requiredSize, nullptr, 0, strA.data(), _TRUNCATE);
		if (requiredSize > 0) 
		{			
			strW.resize(requiredSize - 1);
			mbstowcs_s(nullptr, &strW[0], requiredSize, strA.data(), _TRUNCATE);
		}

		return strW;
	}

public:
	// 초기화 시 제한값을 설정한다.
	// - limitation: 제한값
	static void OnInit(int64_t limitation)
	{
		UsageMeterData& data = GetData();
		data.mLimitation += limitation;
	}

public:
	// 모든 통계를 초기화한다.
	static void OnReset()
	{
		UsageMeterData& data = GetData();
		data.mLimitation = 0;
		data.mIncrease = 0;
		data.mDecrease = 0;
	}

	// 제한값을 변경한다.
	// - val: 새로운 제한값
	static void OnChangeLimitation(int64_t val)
	{
		UsageMeterData& data = GetData();
		data.mLimitation = val;
	}

	// 증가 횟수를 증가시킨다.
	// - val: 증가량 (기본값: 1)
	static void OnIncrease(int64_t val = 1)
	{
		UsageMeterData& data = GetData();
		data.mIncrease += val;
	}

	// 감소 횟수를 증가시킨다.
	// - val: 감소량 (기본값: 1)
	static void OnDecrease(int64_t val = 1)
	{
		UsageMeterData& data = GetData();
		data.mDecrease += val;
	}
};
