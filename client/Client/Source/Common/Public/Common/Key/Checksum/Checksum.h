// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 체크섬 구조체 (16바이트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 16바이트(4바이트 * 4)의 체크섬 결과를 저장하고 비교하는 구조체.
// - 책임: MD5 체크섬 값 저장 및 비교
#pragma pack(push, 1)
struct Checksum
{
	uint32_t checksum0 = 0;  // 체크섬 0 (4바이트)
	uint32_t checksum1 = 0;  // 체크섬 1 (4바이트)
	uint32_t checksum2 = 0;  // 체크섬 2 (4바이트)
	uint32_t checksum3 = 0;  // 체크섬 3 (4바이트)

	Checksum() = default;
	Checksum(const std::array<uint32_t, 4>& checksumArray)
	{
		checksum0 = checksumArray[0];
		checksum1 = checksumArray[1];
		checksum2 = checksumArray[2];
		checksum3 = checksumArray[3];
	}
	Checksum& operator=(const std::array<uint32_t, 4>& checksumArray)
	{
		checksum0 = checksumArray[0];
		checksum1 = checksumArray[1];
		checksum2 = checksumArray[2];
		checksum3 = checksumArray[3];
		return *this;
	}
	// 두 체크섬이 같은지 비교한다.
	// - other: 비교할 체크섬
	// - return: 같으면 true
	bool operator==(const Checksum& other)
	{
		return
			(checksum0 == other.checksum0) &&
			(checksum1 == other.checksum1) &&
			(checksum2 == other.checksum2) &&
			(checksum3 == other.checksum3);
	}
	// 두 체크섬이 다른지 비교한다.
	// - other: 비교할 체크섬
	// - return: 다르면 true
	bool operator!=(const Checksum& other)
	{
		return
			(checksum0 != other.checksum0) ||
			(checksum1 != other.checksum1) ||
			(checksum2 != other.checksum2) ||
			(checksum3 != other.checksum3);
	}
};
#pragma pack(pop)
