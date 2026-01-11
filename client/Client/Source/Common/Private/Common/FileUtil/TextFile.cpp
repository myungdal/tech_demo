// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/FileUtil/TextFile.h"

#if defined(__UNREAL__)
#	include "HAL/PlatformFilemanager.h"
#	include "Misc/Paths.h"
#	include "Misc/FileHelper.h"
#	include "Serialization/Archive.h"
#	include "Containers/UnrealString.h"
#	include "Containers/StringConv.h"
#else
#	include "Common/FileUtil/FileUtil.h"
#endif

#if defined(__UNREAL__)
TextFile::TextFile(const wchar_t* filename)
{
	mFilename = FPaths::Combine(FPaths::ProjectLogDir(), filename);
	UpdateFileHandle();
}
TextFile::~TextFile()
{
	if (mFileArchive)
	{
		mFileArchive->Flush();
		mFileArchive->Close();
		delete mFileArchive;
		mFileArchive = nullptr;
	}
}
void TextFile::UpdateFileHandle()
{
	// 파일명에 번호를 붙여서 중복되지 않는 파일을 찾는다 (최대 1000개)
	for (size_t i = 0; i < 1000; ++i)
	{
		std::wstring stdFileName(*mFilename);
		std::wstring formattedFilename = std::format(L"{}_{:03}.log", stdFileName, i);
		FString filePath(formattedFilename.c_str());
		
		if (IFileManager::Get().FileExists(*filePath))
		{
			continue;
		}
		
		mFileArchive = IFileManager::Get().CreateFileWriter(*filePath, FILEWRITE_Append | FILEWRITE_AllowRead);

		if (mFileArchive)
		{
			break;
		}
	}
}

void TextFile::WriteToFile(const std::wstring_view text)
{
	if (mFileArchive)
	{
		FString message(text.data(), text.size());
		FString messageWithNewline = message;

		FTCHARToUTF8 UTF8Converter(*messageWithNewline);
		mFileArchive->Serialize((void*)UTF8Converter.Get(), UTF8Converter.Length());		
	}
}

#else
TextFile::TextFile(const wchar_t* filename)
	:
	mFilename(filename)
{
	UpdateFileHandle();
}

TextFile::~TextFile()
{
	if (INVALID_HANDLE_VALUE != mFileHandle)
	{
		FlushFileBuffers(mFileHandle);
		CloseHandle(mFileHandle);
	}
}

void TextFile::UpdateFileHandle()
{
	// 기존 핸들이 있으면 닫는다
	if (INVALID_HANDLE_VALUE != mFileHandle)
	{
		FlushFileBuffers(mFileHandle);
		CloseHandle(mFileHandle);
	}
	else
	{
		setlocale(LC_ALL, "");
	}

	// 파일명에 번호를 붙여서 중복되지 않는 파일을 찾는다 (최대 1000개)
	for (size_t i = 0; i < 1000; ++i)
	{
		std::wstring filename = std::format(L"{}_{:03}.log", mFilename, i);
		mFileHandle = FileUtil::CreateTextFile(filename.data());

		if (INVALID_HANDLE_VALUE != mFileHandle)
			break;
	}
}

void TextFile::WriteToFile(const std::wstring_view text)
{
	DWORD writtingSize = static_cast<DWORD>(sizeof(wchar_t) * text.length());
	DWORD written = 0;
	WriteFile(mFileHandle, text.data(), writtingSize, &written, nullptr);

	_ASSERT_CRASH(written == writtingSize);
}
#endif
