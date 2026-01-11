// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "LogWriterConsole.h"


LogWriterConsole::LogWriterConsole(AppType appType, AppArg appArg, AppId mAppId)
{
	{
		constexpr const std::pair<int, int> colRow[_IDX(AppType::MAX)] =
		{
			std::make_pair(1, 0),	// BRIDGE_SERVER = 0,
			std::make_pair(0, 1),	// DB_SERVER = 1,
			std::make_pair(0, 2),	// FRONT_SERVER = 2,
			std::make_pair(1, 1),	// GAME_SERVER = 3,
			std::make_pair(2, 0),	// LOG_SERVER = 4,
			std::make_pair(0, 0),	// MAIN_SERVER = 5,
			std::make_pair(2, 2),	// SHELL = 6,
			std::make_pair(2, 1)	// BOT = 7,
		};
		const auto& [c, r] = colRow[_IDX(appType)];

		const HWND hwndConsole = GetConsoleWindow();

		MoveWindow(hwndConsole, CONSOLE_WIDTH * c, CONSOLE_HEIGHT * r, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
	}

	{
		std::ios::sync_with_stdio(true);
		std::wcin.tie(nullptr);
		std::wcout.tie(nullptr);

		mConsoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
		mConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD inputConsoleMode = 0;
		GetConsoleMode(mConsoleInputHandle, &inputConsoleMode);
		SetConsoleMode(mConsoleInputHandle, (inputConsoleMode & ~ENABLE_QUICK_EDIT_MODE));
		SetConsoleTextAttribute(mConsoleOutputHandle, (0x0001 | 0x0002 | 0x0004 | 0x0008));

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 12;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_THIN;
		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(mConsoleOutputHandle, FALSE, &cfi);
	}

	{
		auto [titleStr, titleStrLen] = StringUtil::FormatStr(L"{}_{}({})", appType, appArg, mAppId);

		SetConsoleTitleW(*titleStr);
	}
}

LogWriterConsole::~LogWriterConsole()
{
	fflush(stdout);
	Sleep(1000);
}

void LogWriterConsole::WriteToConsole(const Log* log)
{
	while (nullptr != log)
	{
		if (log->mColor < _SHOW_LOG_COLOR_LEVEL)
		{
			log = log->GetNext();
			continue;
		}
		
		if (mConsoleFontColor != log->mColor)
		{
			mConsoleFontColor = log->mColor;

			constexpr uint16_t Color[]
			{
				(0x0001 | 0x0002 | 0x0004 | 0x0008),
				(0x0002 | 0x0004 | 0x0008),
				(0x0002 | 0x0008),
				(0x0004 | 0x0008)
			};

			SetConsoleTextAttribute(mConsoleOutputHandle, Color[log->mColor]);
		}

		std::wcout << (log->mStr + log->mOffsetForConsoleStr);

		log = log->GetNext();
	}
}
