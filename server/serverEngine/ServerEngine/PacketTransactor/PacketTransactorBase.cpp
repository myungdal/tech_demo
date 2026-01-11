// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "PacketTransactorBase.h"


void PacketTransactorBase::Run()
{
	mResult = OnPreValidate();
	if (HasError())
	{
		OnError();
		return;
	}
	OnPreUpdate();

	mResult = OnUpdate();
	if (HasError())
	{
		if (!OnRollback())
		{
			_DEBUG_RED;
		}

		OnError();
		return;
	}

	if (!OnCommit())
	{
		_DEBUG_RED;
	}

	OnFinish();

	OnLog();
}
