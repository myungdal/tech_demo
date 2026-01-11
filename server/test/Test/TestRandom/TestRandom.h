// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"


class TestRandom final : public TestAppBase
{
public:
	explicit TestRandom(AppArg appArg) : TestAppBase(appArg) {}

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;
};

inline std::shared_ptr<TestRandom> gTestRandom = nullptr;
