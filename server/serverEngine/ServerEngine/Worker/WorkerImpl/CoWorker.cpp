// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "CoWorker.h"

import ServerEngineWorker;
import ServerEngineCore;


CoWorker::CoWorker()
{
	mWorkerArray.fill(nullptr);
	for (std::weak_ptr<Worker>*& workerWeakPtr : mWorkerArray)
	{
		workerWeakPtr = _NEW<std::weak_ptr<Worker>>();	
	}
}
CoWorker::~CoWorker()
{
	for (std::weak_ptr<Worker>* workerWeakPtr : mWorkerArray)
	{
		_DELETE(workerWeakPtr);
	}
}
void CoWorker::PushWorker(std::shared_ptr<Worker> worker)
{
	size_t pos = (++mPushPos - 1) & POOL_SIZE_MASK;

	do 
	{
		std::weak_ptr<Worker>* workerWeakPtr =
			reinterpret_cast<std::weak_ptr<Worker>*>(InterlockedExchangePointer(
				reinterpret_cast<void**>(&mWorkerArray[pos]),
				nullptr
			));

		if (workerWeakPtr)
		{
			std::shared_ptr<Worker> prev = workerWeakPtr->lock();

			// 비어 있다면 성공
			if (!prev)
			{
				(*workerWeakPtr) = worker;

				++mWorkerCnt;

				InterlockedExchangePointer(
					reinterpret_cast<void**>(&mWorkerArray[pos]),
					reinterpret_cast<void*>(workerWeakPtr)
				);

				return;
			}


			InterlockedExchangePointer(
				reinterpret_cast<void**>(&mWorkerArray[pos]),
				reinterpret_cast<void*>(workerWeakPtr)
			);
		}

		_YIELD_SPIN;

	} while (true);
}

void CoWorker::Run()
{
	size_t pos = mPopPos.load() & POOL_SIZE_MASK;

	std::weak_ptr<Worker>* workerWeakPtr =
		reinterpret_cast<std::weak_ptr<Worker>*>(InterlockedExchangePointer(
			reinterpret_cast<void**>(&mWorkerArray[pos]),
			nullptr
		));

	if (workerWeakPtr)
	{
		std::shared_ptr<Worker> worker = workerWeakPtr->lock();

		if (!worker)
		{
			InterlockedExchangePointer(
				reinterpret_cast<void**>(&mWorkerArray[pos]),
				workerWeakPtr
			);

			return;
		}
		
		++mPopPos;

		workerWeakPtr->reset();

		InterlockedExchangePointer(
			reinterpret_cast<void**>(&mWorkerArray[pos]),
			workerWeakPtr
		);

		--mWorkerCnt;

		worker->TryWork();
	}
}
