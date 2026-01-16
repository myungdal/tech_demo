// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ClientUtil/ClientFileUtil.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Internationalization/Regex.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


TArray<uint8_t> FClientFileUtil::LoadByteArrayFile(const FString& filename)
{
	TArray<uint8_t> result;

	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), filename);
	FString pathPart, disregard;
	FPaths::Split(fullPath, pathPart, disregard, disregard);
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (false == platformFile.CreateDirectoryTree(*pathPart))
		return result;

	FFileHelper::LoadFileToArray(result, *fullPath);
	return result;
}

bool FClientFileUtil::SaveByteArrayFile(const FString& filename, const TArray<uint8_t>& byteArray)
{
	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), filename);
	FString pathPart, disregard;
	FPaths::Split(fullPath, pathPart, disregard, disregard);
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	return
		platformFile.CreateDirectoryTree(*pathPart) &&
		FFileHelper::SaveArrayToFile(byteArray, *fullPath);
}

FString FClientFileUtil::LoadTxtFile(const FString& filename)
{
	FString jsonStr;

	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), filename);
	FString pathPart, disregard;
	FPaths::Split(fullPath, pathPart, disregard, disregard);
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (false == platformFile.CreateDirectoryTree(*pathPart))
		return jsonStr;

	if (false == FFileHelper::LoadFileToString(jsonStr, *fullPath))
		return jsonStr;

	return jsonStr;
}

TSharedPtr<FJsonObject> FClientFileUtil::LoadJsonFile(const FString& filename)
{
	FString jsonStr = FClientFileUtil::LoadTxtFile(filename);

	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonRoot;
	if (false == FJsonSerializer::Deserialize(jsonReader, jsonRoot))
		return nullptr;

	return jsonRoot;
}

bool FClientFileUtil::SaveTxtFile(const FString& filename, const FString& json)
{
	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), filename);
	FString pathPart, disregard;
	FPaths::Split(fullPath, pathPart, disregard, disregard);
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	return
		platformFile.CreateDirectoryTree(*pathPart) &&
		FFileHelper::SaveStringToFile(*json, *fullPath);
}

void FClientFileUtil::ReloadDebugInfoFile()
{
	// DebugInfoDef.inl 파일에서 DEBUG_VAR_DEF(name, type, value) 패턴 파싱
	// 주의: ProjectSavedDir 기준이 아닌 ProjectDir 기준으로 읽어야 함
	const FString filename = TEXT("Source/MmoSync/Public/MmoSync/DebugControlTool/DebugInfoDef.inl");
	const FString fullPath = FPaths::Combine(FPaths::ProjectDir(), filename);
	
	FString fileContent;
	if (!FFileHelper::LoadFileToString(fileContent, *fullPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugInfoDef.inl 파일 읽기 실패: %s"), *fullPath);
		return;
	}

	// 정규식: DEBUG_VAR_DEF(name, type, value) 또는 DEBUG_VAR_DEF_EXPR(name, type, expr)
	// 캡처: (1)변수명, (2)타입, (3)값/표현식
	const FRegexPattern pattern(
		TEXT("DEBUG_VAR_DEF(?:_EXPR)?\\s*\\(\\s*(\\w+)\\s*,\\s*(\\w+)\\s*,\\s*([^)]+)\\s*\\)")
	);
	FRegexMatcher matcher(pattern, fileContent);

	// 표현식 파싱용 정규식: (baseValue << shiftValue) 또는 (baseValue >> shiftValue)
	const FRegexPattern exprPattern(
		TEXT("\\(?\\s*(-?\\d+)\\s*(<<|>>)\\s*(\\d+|\\w+)\\s*\\)?")
	);

	// 파일 내 모든 DEBUG_VAR_DEF 변수를 순회하며 gDebugInfo에 등록
	while (matcher.FindNext())
	{
		const FString varName = matcher.GetCaptureGroup(1);      // 변수명
		const FString typeName = matcher.GetCaptureGroup(2);     // 타입
		FString valueExpr = matcher.GetCaptureGroup(3);          // 값 또는 표현식
		valueExpr = valueExpr.TrimStartAndEnd();

		// 타입 정보 저장
		DebugInfo::sTypeMap[*varName] = *typeName;

		int64 finalValue = 0;

		// 표현식 파싱 시도 (예: (3000 << CELL_SIZE_DESIGN_FACTOR))
		FRegexMatcher exprMatcher(exprPattern, valueExpr);
		if (exprMatcher.FindNext())
		{
			const FString baseValueStr = exprMatcher.GetCaptureGroup(1);
			const FString op = exprMatcher.GetCaptureGroup(2);
			const FString shiftStr = exprMatcher.GetCaptureGroup(3);

			int64 baseValue = FCString::Atoi64(*baseValueStr);
			int32 shiftAmount = 0;

			// 시프트 값이 숫자인지 상수명인지 확인
			if (FChar::IsDigit(shiftStr[0]))
			{
				shiftAmount = FCString::Atoi(*shiftStr);
			}
			else
			{
				// DebugInfo::sKnownConstants에서 상수 조회
				const std::wstring constKey(*shiftStr);
				auto it = DebugInfo::sKnownConstants.find(constKey);
				if (it != DebugInfo::sKnownConstants.end())
				{
					shiftAmount = static_cast<int32>(it->second);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("DebugInfo 파싱 오류: 알 수 없는 상수 '%s' (변수: %s)"), *shiftStr, *varName);
					continue;
				}
			}

			if (op == TEXT("<<"))
				finalValue = baseValue << shiftAmount;
			else if (op == TEXT(">>"))
				finalValue = baseValue >> shiftAmount;
		}
		else
		{
			// 단순 숫자 값
			finalValue = FCString::Atoi64(*valueExpr);
		}

		// DEBUG_ENTITY_ID는 디버깅 중 수동 설정한 값을 보존
		if (varName == TEXT("DEBUG_ENTITY_ID"))
		{
			if (DebugInfo::DEBUG_ENTITY_ID != -1)
				continue;
		}

#if 0 // 디버그: 값 변경 로그
		UE_LOG(LogTemp, Log, TEXT("DebugInfo: %s = %lld"), *varName, finalValue);
#endif

		gDebugInfo.LoadDebugInfoVar(*varName, finalValue);
	}
}

//void FClientFileUtil::OpenDebugInfoFile()
//{
//	FString FolderPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Saved"));
//	FolderPath = TEXT("file:///") + FolderPath;
//
//	FPlatformProcess::LaunchURL(*FolderPath, nullptr, nullptr);
//}

//
//void FClientFileUtil::ResetDebugInfoFile()
//{
//	const FString filename = TEXT("../Source/MmoSync/Public/MmoSync/DebugControlTool/DebugInfo.hpp");
//
//	FString cppStr;
//	const DebugInfo::Map& refMap = gDebugInfo.RefMap();
//
//	for (auto it = refMap.begin(); it != refMap.end(); ++it)
//	{
//		if (std::next(it) == refMap.end())
//		{
//			cppStr += FString::Printf(TEXT("\t{ L\"%s\", %lld }\n"), it->first.c_str(), it->second);
//		}
//		else
//		{
//			cppStr += FString::Printf(TEXT("\t{ L\"%s\", %lld },\n"), it->first.c_str(), it->second);
//		}
//	}
//
//	FClientFileUtil::SaveTxtFile(filename, cppStr);
//}
