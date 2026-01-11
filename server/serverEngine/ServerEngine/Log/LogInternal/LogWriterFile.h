// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LogWriterFile
{
protected:
	HANDLE mLogFileHandle = INVALID_HANDLE_VALUE;

	std::wstring mPath = L"./";
	std::wstring mOwner = L"";
	int mLogFileYear = 0;
	int mLogFileMon = 0;
	int mLogFileDay = 0;
	int mLogFileHour = 0;
	int mLogFileMin = 0;

protected:
	static constexpr size_t MAX_FILE_SIZE = 512 * 1024 * 1024; //500메가
	size_t mWrittenFileSize = 0;

public:
	LogWriterFile(const wchar_t* path, const wchar_t* owner);
	virtual ~LogWriterFile();

private:
	void MoveToBackupFolder();
	void UpdateFileHandle(const tm& localTm);

public:
	void WriteToFile(const Log* log);
};
