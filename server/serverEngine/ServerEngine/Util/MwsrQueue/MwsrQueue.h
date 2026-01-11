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

template <typename _Type>
class MwsrQueue
{
private:	
	SLIST_HEADER mHead;
	std::atomic<size_t> mSize{ 0 };

public:
	MwsrQueue()
	{
		InitializeSListHead(&mHead);

		mSize = 0;
	}

public:
	void Push(_Type* item)
	{
		_ASSERT_CRASH(0 == (reinterpret_cast<std::uintptr_t>(item) % MEMORY_ALLOCATION_ALIGNMENT));

		item->Next = nullptr;

		InterlockedPushEntrySList(&mHead, item);

		++mSize;
	}

	_Type* Pop(OUT size_t& count)
	{
		count = 0;

		SLIST_ENTRY* head = InterlockedFlushSList(&mHead);
		
		if (nullptr == head)
		{
			return nullptr;
		}

		SLIST_ENTRY* prev = nullptr;
		
		do
		{
			SLIST_ENTRY* temp = prev;

			prev = head;
			head = head->Next;
			prev->Next = temp;

			++count;

		} while (head);

		mSize -= count;

		return static_cast<_Type*>(prev);
	}
};
