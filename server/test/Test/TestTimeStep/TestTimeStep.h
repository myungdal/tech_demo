// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"
#include "MmoSync/Util/TimeStep/TimeStepPlayer.h"
#include "Common/MemPool/DynamicPool.h"
#include <deque>

using CommandQueue = std::deque<WorldUserId>;
using TimeStepPlayerType = TimeStepPlayer<CommandQueue>;
using CommandQueuePool = DynamicPool<CommandQueue>;

class TestTimeStep final : public TestAppBase
{
private:
	TimeStepPlayerType mTimeStepPlayer;
	CommandQueuePool mCommandQueuePool;
	int mTestCount = 0;
	int mPassCount = 0;

public:
	explicit TestTimeStep(AppArg appArg) : TestAppBase(appArg), mTimeStepPlayer(TIME_STEP_INTERVAL, CLOCK_NOW, false) {}

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;
};

inline std::shared_ptr<TestTimeStep> gTestTimeStep = nullptr;
