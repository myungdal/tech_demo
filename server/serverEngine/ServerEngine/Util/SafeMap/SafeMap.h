// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _KeyType, typename _ValueType>
class SafeMap
{
private:
	Lock mLock;

private:
	using Container = std::unordered_map<_KeyType, _ValueType>;

	Container mContainer;
	std::atomic<size_t> mCount = 0;

public:
	bool Insert(_KeyType key, _ValueType& value)
	{
		WriteLock lock(mLock);

		auto [_, newly] =  mContainer.emplace(key, value);

		if (newly)
		{
			mCount.fetch_add(1);
		}

		return newly;
	}

	bool Remove(_KeyType key)
	{
		WriteLock lock(mLock);

		const size_t count = mContainer.erase(key);

		if (0 != count)
		{
			mCount.fetch_sub(1);
			return true;
		}

		return false;
	}

	_ValueType& Find(_KeyType key)
	{
		WriteLock lock(mLock);

		auto it = mContainer.find(key);
		if (mContainer.end() == it)
			return nullptr;

		return it->second;
	}

	size_t Count() const
	{
		return mCount.load();
	}
};
