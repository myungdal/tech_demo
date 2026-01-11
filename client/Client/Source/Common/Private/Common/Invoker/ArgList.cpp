// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Invoker/ArgList.h"


ArgList::ArgList(const std::wstring& argString)
{
	// [WHY] 공백으로 인자 구분 (depth0)
	std::wstringstream stream0(argString);
	std::wstring depth0;
	while (stream0 >> depth0)
	{
		List list;

		// [WHY] 쉼표로 배열 요소 구분 (depth1)
		std::wstringstream stream1(depth0);
		std::wstring depth1;
		while (std::getline(stream1, depth1, L','))
		{
			// [WHY] 빈 문자열은 무시
			if(0 != depth1.length())
				list.emplace_back(depth1);
		}

		mTable.emplace_back(std::move(list));
	}
}

std::wstring ArgList::GetFullString()
{
	std::wstring result = L"";

	if (0 < mTable.size())
	{
		for (const List& list : mTable)
		{
			if (0 < list.size())
			{
				for (const std::wstring& arg : list)
				{
					result += arg + L",";
				}
				result.pop_back();
			}
			else
			{
				result += L",";
			}

			result += L" ";
		}
		result.pop_back();
	}

	return result;
}

std::wstring ArgList::GetFullStringExceptFirst()
{
	std::wstring result = L"";

	auto it = mTable.begin();
	if (it != mTable.end())
	{
		++it;
		for (; it != mTable.end(); ++it) 
		{
			auto& list = *it;
			if (0 < list.size())
			{
				for (const std::wstring& arg : list)
				{
					result += arg + L",";
				}
				result.pop_back();
			}
			else
			{
				result += L",";
			}

			result += L" ";
		}
		result.pop_back();
	}

	return result;
}

size_t ArgList::GetSize() const
{
	size_t size = mTable.size();
	return size;
}

bool ArgList::Has(int8_t i) const
{
	const List* list = GetList(i);
	return (nullptr != list);
}

const wchar_t* ArgList::GetStr(int8_t i) const
{
	const List* list = GetList(i);
	if (nullptr == list)
		return L"";

	return (*list)[0].c_str();
}

size_t ArgList::GetArraySize(int8_t i) const
{
	const List* list = GetList(i);
	if (nullptr == list)
		return 0;

	return list->size();
}

const wchar_t* ArgList::GetArrayStr(int8_t i, int8_t j) const
{
	const List* list = GetList(i);
	if (nullptr == list)
		return L"";

	if (list->size() <= j)
		return L"";

	return (*list)[j].c_str();
}

const ArgList::List* ArgList::GetList(int8_t i) const
{
	if (mTable.size() <= i)
		return nullptr;

	const List& list = mTable[i];

	if (true == list.empty())
		return nullptr;

	return &list;
}

