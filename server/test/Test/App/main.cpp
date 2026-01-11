// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "Test/TestFlowControlScript/TestFlowControlScript.h"
#include "Test/TestDeterministicRandom/TestDeterministicRandom.h"
#include "Test/TestSmooth/TestSmooth.h"
#include "Test/TestTimeStep/TestTimeStep.h"
#include "Test/TestAdaptiveRandom/TestAdaptiveRandom.h"
#include "Test/TestMt19937Random/TestMt19937Random.h"
#include "Test/TestNonReplacementSampler/TestNonReplacementSampler.h"
#include "Test/TestRandMark40/TestRandMark40.h"
#include "Test/TestMapGridGenerator/TestMapGridGenerator.h"
#include "Test/TestRatioTable/TestRatioTable.h"
#include "Test/TestBadWordFilter/TestBadWordFilter.h"
#include "Test/TestSocketStateMachine/TestSocketStateMachine.h"
#include "Test/TestLock/TestLock.h"
#include "Test/TestCoWorker/TestCoWorker.h"
#include "Test/TestTimeStepJitter/TestTimeStepJitter.h"
#include "Test/TestUserCacheDiff/TestUserCacheDiff.h"


int wmain(MAYBE_UNUSED int argc, MAYBE_UNUSED wchar_t* argv[])
{
	_PERFORMANCE_TRACE_START;

	gLogQueue = std::make_shared<LogQueue>();

	const long appArg = (argc > 1) ? wcstol(argv[1], nullptr, 10) : 0;

	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestFlowControlScript>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestDeterministicRandom>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestSmooth>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestTimeStep>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestAdaptiveRandom>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestMt19937Random>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestNonReplacementSampler>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestRandMark40>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestRatioTable>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestBadWordFilter>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	{
		std::shared_ptr<TestAppBase> testApp = std::make_shared<TestTimeStepJitter>(appArg);
		testApp->ResumeWorker();
		testApp->Run();
		testApp->SuspendWorker();
	}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestMapGridGenerator>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestSocketStateMachine>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestLock>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestCoWorker>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}
	//{
	//	std::shared_ptr<TestAppBase> testApp = std::make_shared<TestUserCacheDiff>(appArg);
	//	testApp->ResumeWorker();
	//	testApp->Run();
	//	testApp->SuspendWorker();
	//}

	return 0;
}
