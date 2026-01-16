// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "PerformanceRecorderContainer.h"


PerformanceRecorderContainer::PerformanceRecorderContainer()
{
	tPerformanceRecorder = _NEW<PerformanceRecorder>();
}
PerformanceRecorderContainer::~PerformanceRecorderContainer()
{
	if (nullptr != tPerformanceRecorder)
	{
		_DELETE(tPerformanceRecorder);
		tPerformanceRecorder = nullptr;
	}
}
