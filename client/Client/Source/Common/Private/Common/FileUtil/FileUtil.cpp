// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/FileUtil/FileUtil.h"

#if defined(__UNREAL__)
#include <fstream>

#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#endif


#if defined(__UNREAL__)
void FileUtil::EnsureDirectoryExists(const std::wstring& path)
{
	// [WHY] 언리얼 엔진에서는 FString을 사용해야 하므로 변환 필요
	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), FString(path.c_str()));
	FString directory, filename, extension;
	FPaths::Split(fullPath, directory, filename, extension);

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	// [NOTE] 반환값은 bool이지만, 여기서는 성공 여부에 대해 별도 처리하지 않음
	platformFile.CreateDirectoryTree(*directory);
}

std::ofstream FileUtil::CreateOutputFileStream(const char* filename)
{
	std::wstring filenameW;
	size_t requiredSize = 0;
	// [WHY] UTF-8을 UTF-16으로 변환하기 위해 필요한 버퍼 크기 계산
	mbstowcs_s(&requiredSize, nullptr, 0, filename, _TRUNCATE);
	if (requiredSize > 0)
	{
		filenameW.resize(requiredSize - 1);
		mbstowcs_s(nullptr, &filenameW[0], requiredSize, filename, _TRUNCATE);
	}

	EnsureDirectoryExists(filenameW);
	return std::ofstream(filename);
}
#else
void FileUtil::EnsureDirectoryExists(const std::wstring& path)
{
	std::filesystem::path directoryPath = std::filesystem::path(path).parent_path();

	if (!std::filesystem::exists(directoryPath))
	{
		try
		{
			std::filesystem::create_directories(directoryPath);
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			// [EDGE-CASE] 디렉터리 생성 실패 시 에러 출력
			std::wcerr << L"Error creating directory: " << e.what() << std::endl;
		}
	}
}

std::ofstream FileUtil::CreateOutputFileStream(const char* filename)
{
	std::wstring filenameW{ L"" };

	// [WHY] UTF-8을 UTF-16으로 변환하기 위해 필요한 버퍼 크기 계산
	size_t requiredSize = 0;
	mbstowcs_s(&requiredSize, nullptr, 0, filename, _TRUNCATE);
	if (requiredSize > 0)
	{
		filenameW.resize(requiredSize - 1);
		mbstowcs_s(nullptr, &filenameW[0], requiredSize, filename, _TRUNCATE);
	}

	FileUtil::EnsureDirectoryExists(filenameW);
	return std::ofstream(filename);
}

HANDLE FileUtil::CreateTextFile(const wchar_t* filename)
{
	FileUtil::EnsureDirectoryExists(filename);

	const HANDLE fileHandle = CreateFileW(
		filename,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (INVALID_HANDLE_VALUE == fileHandle)
		return INVALID_HANDLE_VALUE;

	// [WHY] 빈 파일인 경우 UTF-16 LE BOM 추가
	const DWORD fileSize = GetFileSize(fileHandle, nullptr);
	if (0 == fileSize)
	{
		constexpr uint8_t bom[2] = { 0xff, 0xfe };
		DWORD written = 0;
		WriteFile(fileHandle, bom, 2, &written, nullptr);
		FlushFileBuffers(fileHandle);
	}

	SetFilePointer(fileHandle, 0, nullptr, FILE_END);

	return fileHandle;
}
#endif
