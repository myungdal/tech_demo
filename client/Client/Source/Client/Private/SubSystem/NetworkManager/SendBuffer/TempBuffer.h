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

class CLIENT_API FTempBuffer
{
private:
	uint8_t* mBuffer = nullptr;

public:
	FTempBuffer();
	virtual ~FTempBuffer();

public:
	uint8_t* operator*() { return mBuffer; }
};
