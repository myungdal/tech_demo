// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"
#include "MmoSync/Map/Visualization/DebugMapSystem.h"


class TestMapGridGenerator : public TestAppBase
{
public:
	explicit TestMapGridGenerator(long appArg);

public:
	void OnInitialize(CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;

private:
	// Demo methods
	void RunDungeonDemo(std::vector<std::wstring>& outFiles);
	void RunFieldDemo(std::vector<std::wstring>& outFiles);
	
	// Statistics methods
	void PrintStatistics(const std::wstring& mapType, const std::vector<std::wstring>& mapFiles);
};
