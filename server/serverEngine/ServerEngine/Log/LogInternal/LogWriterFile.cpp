// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "LogWriterFile.h"

#include "Common/FileUtil/FileUtil.h"


LogWriterFile::LogWriterFile(const wchar_t* path, const wchar_t* owner)
	:
	mPath(path),
	mOwner(owner)
{
	tm localTm{};
	_localtime64_s(&localTm, &tClock.GetLocalNowTt());
	UpdateFileHandle(localTm);
	MoveToBackupFolder();
}
LogWriterFile::~LogWriterFile()
{
	if (INVALID_HANDLE_VALUE != mLogFileHandle)
	{
		FlushFileBuffers(mLogFileHandle);
		CloseHandle(mLogFileHandle);
	}
}
void LogWriterFile::MoveToBackupFolder()
{
	// 소스 폴더.
	auto [sourcePathStr, sourcePathLen] = StringUtil::FormatStr(
		L"../../log/{}/",
		mPath
	);
	std::filesystem::path sourcePath = std::filesystem::path(*sourcePathStr);

	// 오늘 날짜 파일 패턴.
	auto [sourcePatternStr, sourcePatternLen] = StringUtil::FormatStr(
		L".*{:02}년{:02}월{:02}일.*",
		mLogFileYear, mLogFileMon, mLogFileDay
	);

	// 백업 폴더
	auto [backupDir, backupDirLen] = StringUtil::FormatStr(
		L"../../log_backup/{}",
		mPath
	);

	// 백업 폴더 생성
	FileUtil::EnsureDirectoryExists(*backupDir);

	// 디렉터리 순회
	for (const auto& entry : std::filesystem::directory_iterator(sourcePath)) 
	{
		if (entry.is_regular_file()) 
		{
			std::wstring fileName = entry.path().filename().wstring();

			// 패턴과 일치 여부 확인
			if (!std::regex_match(fileName, std::wregex(*sourcePatternStr)))
			{
				// 패턴에 일치하지 않는 파일 이동
				std::filesystem::path targetPath = std::filesystem::path(*backupDir) / entry.path().filename();
				std::filesystem::rename(entry.path(), targetPath);
			}
		}
	}
}

void LogWriterFile::UpdateFileHandle(const tm& localTm)
{
	if ((MAX_FILE_SIZE > mWrittenFileSize) &&
		(mLogFileYear == localTm.tm_year - 100) &&
		(mLogFileMon == localTm.tm_mon + 1) &&
		(mLogFileDay == localTm.tm_mday) &&
		(mLogFileHour == localTm.tm_hour) &&
		(mLogFileMin == (localTm.tm_min / 15) * 15))
	{
		return;
	}

	if (INVALID_HANDLE_VALUE != mLogFileHandle)
	{
		FlushFileBuffers(mLogFileHandle);
		CloseHandle(mLogFileHandle);
	}
	else
	{
		setlocale(LC_ALL, "");
	}

	mWrittenFileSize = 0;

	mLogFileYear = localTm.tm_year - 100;
	mLogFileMon = localTm.tm_mon + 1;
	mLogFileDay = localTm.tm_mday;
	mLogFileHour = localTm.tm_hour;
	mLogFileMin = (localTm.tm_min / 15) * 15;

	for (size_t i = 0; i < 1000; ++i)
	{
		auto [logFilename, logFilenameLen] = StringUtil::FormatStr(
			L"../../log/{}/[{}]{:02}년{:02}월{:02}일{:02}시{:02}분_{:03}.log",
			mPath, mOwner, mLogFileYear, mLogFileMon, mLogFileDay, mLogFileHour, mLogFileMin, i
		);

		mLogFileHandle = FileUtil::CreateTextFile(*logFilename);

		if (INVALID_HANDLE_VALUE != mLogFileHandle)
			break;
	}
}

void LogWriterFile::WriteToFile(const Log* log)
{
	while (nullptr != log)
	{
		if (INVALID_HANDLE_VALUE == mLogFileHandle)
			break;

		DWORD writtingSize = static_cast<DWORD>(sizeof(wchar_t) * log->mStrLen);
		DWORD written = 0;
		WriteFile(mLogFileHandle, log->mStr, writtingSize, &written, nullptr);

		_ASSERT_CRASH(written == writtingSize);

		mWrittenFileSize += written;

		log = log->GetNext();
	}
}
