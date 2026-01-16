// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


#if defined(__UNREAL__)
#include "Containers/UnrealString.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 텍스트 파일 쓰기 클래스 (언리얼 엔진)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class COMMON_API TextFile
{
protected:
	FArchive* mFileArchive = nullptr;  // 파일 아카이브 핸들
	FString mFilename{ "Test" };        // 파일명

public:
	TextFile(const wchar_t* path);
	virtual ~TextFile();

private:
	void UpdateFileHandle();  // 파일 핸들 갱신

public:
	// 파일에 텍스트를 쓴다.
	// - text: 쓸 텍스트 (UTF-16)
	void WriteToFile(const std::wstring_view text);
};
#else
// 텍스트 파일 쓰기 클래스 (Windows)
class TextFile
{
protected:
	HANDLE mFileHandle = INVALID_HANDLE_VALUE;  // 파일 핸들
	std::wstring mFilename{ L"Test" };          // 파일명

public:
	TextFile(const wchar_t* path);
	virtual ~TextFile();

private:
	void UpdateFileHandle();  // 파일 핸들 갱신

public:
	// 파일에 텍스트를 쓴다.
	// - text: 쓸 텍스트 (UTF-16)
	void WriteToFile(const std::wstring_view text);
};
#endif
