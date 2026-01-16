// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class FJsonObject;

class CLIENTUTIL_API FClientFileUtil
{
public:
	static TArray<uint8_t> LoadByteArrayFile(const FString& filename);
	static bool SaveByteArrayFile(const FString& filename, const TArray<uint8_t>& byteArray);

	static FString LoadTxtFile(const FString& filename);
	static TSharedPtr<FJsonObject> LoadJsonFile(const FString& filename);
	static bool SaveTxtFile(const FString& filename, const FString& json);

	static void ReloadDebugInfoFile();
};
