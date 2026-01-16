// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/FlowControlScript/Types.h"


namespace FlowControlScript
{
	/**
	 * GlobalEnvironment - 전역 환경 관리자
	 * 
	 * FlowControlScript에서 사용할 전역 상수를 관리합니다.
	 * 
	 * 주요 기능:
	 *  - 전역 상수 등록 및 조회
	 *  - C++ 상수를 스크립트에 노출
	 *  - 네임스페이스 스타일 상수 지원 (예: Result::SUCCEEDED)
	 * 
	 * 사용 예시:
	 *  gGlobalEnvironment->RegisterConstant(L"Result::SUCCEEDED", 0);
	 *  gGlobalEnvironment->RegisterConstant(L"Result::FAILED", -1);
	 *  gGlobalEnvironment->RegisterConstant(L"GRID_SIZE", 1024);
	 * 
	 * 스크립트에서 사용:
	 *  if result == Result::SUCCEEDED:
	 *      return GRID_SIZE
	 * 
	 * 주의사항:
	 *  - 전역 상수는 컴파일 시점에 인라인으로 치환됩니다
	 *  - 런타임에 변경할 수 없습니다 (상수)
	 *  - 대소문자를 구분합니다
	 */
	class GlobalEnvironment
	{
	private:
		std::unordered_map<std::wstring, Value> mConstants; // 전역 상수들을 저장하는 맵

	public:
		GlobalEnvironment() = default;
		virtual ~GlobalEnvironment() = default;

	public:
		// 상수를 등록하는 함수
		void RegisterConstant(const std::wstring& name, Value value);

		// 상수를 찾는 함수
		Value FindConstant(const std::wstring& name) const;

		// 상수가 존재하는지 확인
		bool HasConstant(const std::wstring& name) const;

		// 모든 상수를 반환 (디버깅)
		const std::unordered_map<std::wstring, Value>& GetAllConstants() const 
		{
			return mConstants;
		}
	};
} // namespace FlowControlScript

#if defined(__UNREAL__)
COMMON_API extern TSharedPtr<FlowControlScript::GlobalEnvironment> gGlobalEnvironment;
#else
inline std::shared_ptr<FlowControlScript::GlobalEnvironment> gGlobalEnvironment = nullptr;
#endif

