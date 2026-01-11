// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/FlowControlScript/GlobalEnvironment.h"


namespace FlowControlScript
{
	void GlobalEnvironment::RegisterConstant(const std::wstring& name, Value value)
	{
		mConstants[name] = value;
	}

	Value GlobalEnvironment::FindConstant(const std::wstring& name) const
	{
		auto it = mConstants.find(name);
		if (it != mConstants.end())
			return it->second;

		// [EDGE-CASE] 상수가 없으면 INVALID_VALUE 반환
		return INVALID_VALUE;
	}

	bool GlobalEnvironment::HasConstant(const std::wstring& name) const
	{
		return mConstants.find(name) != mConstants.end();
	}
} // namespace FlowControlScript

#if defined(__UNREAL__)
COMMON_API TSharedPtr<FlowControlScript::GlobalEnvironment> gGlobalEnvironment = nullptr;
#endif
