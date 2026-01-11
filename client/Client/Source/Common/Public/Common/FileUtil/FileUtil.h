// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 파일 유틸리티 네임스페이스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__UNREAL__)
namespace FileUtil
{
	// 디렉터리가 존재하지 않으면 생성한다.
	// - path: 디렉터리 경로 (UTF-16)
	void EnsureDirectoryExists(const std::wstring& path);

	// 출력 파일 스트림을 생성한다. 디렉터리가 없으면 자동 생성.
	// - filename: 파일명 (UTF-8)
	// - return: 출력 파일 스트림
	std::ofstream CreateOutputFileStream(const char* filename);
}
#else
namespace FileUtil
{
	// 디렉터리가 존재하지 않으면 생성한다.
	// - path: 디렉터리 경로 (UTF-16)
	void EnsureDirectoryExists(const std::wstring& path);

	// 출력 파일 스트림을 생성한다. 디렉터리가 없으면 자동 생성.
	// - filename: 파일명 (UTF-8)
	// - return: 출력 파일 스트림
	std::ofstream CreateOutputFileStream(const char* filename);

	// UTF-16 텍스트 파일을 생성한다. BOM을 자동 추가.
	// - filename: 파일명 (UTF-16)
	// - return: 파일 핸들. 실패 시 INVALID_HANDLE_VALUE
	HANDLE CreateTextFile(const wchar_t* filename);
}
#endif
