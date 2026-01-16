// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MD5 해시 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MD5 체크섬을 계산하는 클래스.
// - 책임: 바이너리 데이터의 MD5 해시 계산
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API Md5
{
private:
	std::array<uint8_t, 64> mBuffer;      // MD5 처리 버퍼
	std::array<uint32_t, 2> mCount;        // 처리된 바이트 수 카운터
	std::array<uint32_t, 4> mChecksum;     // MD5 체크섬 값 (128비트)

public:	
	Md5();

	// 내부 상태를 초기화한다.
	void Clear();

	// 데이터의 MD5 체크섬을 계산한다.
	// - buffer: 입력 데이터
	// - length: 데이터 크기 (bytes)
	// - return: 체크섬 배열 참조
	const std::array<uint32_t, 4>& GetChecksum(const uint8_t* buffer, size_t length);

	// 체크섬을 16진수 문자열로 변환한다 (UTF-8).
	// - return: 16진수 문자열
	std::string MakeHexStr() const;

	// 체크섬을 16진수 문자열로 변환한다 (UTF-16).
	// - return: 16진수 문자열
	std::wstring MakeHexStrW() const;

protected:
	// 최종 해시를 계산한다.
	void Final();

	// 데이터를 업데이트한다.
	// - in: 입력 데이터
	// - length: 데이터 크기 (bytes)
	void Update(const uint8_t* in, uint32_t length);

	// 64바이트 블록을 변환한다.
	// - block: 변환할 블록
	void Transform(const uint8_t* block);

	// 비트를 왼쪽으로 회전시킨다.
	// - x: 회전할 값
	// - n: 회전 횟수
	// - return: 회전된 값
	uint32_t RotateLeft(uint32_t x, int32_t n) const;

	// MD5 FF 함수.
	void FF(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const;

	// MD5 GG 함수.
	void GG(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const;

	// MD5 HH 함수.
	void HH(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const;

	// MD5 II 함수.
	void II(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const;

	// 32비트 배열을 8비트 배열로 변환한다.
	// - out: 출력 버퍼
	// - in: 입력 배열
	// - length: 변환할 길이
	void Size32ToSize8(OUT uint8_t* out, const uint32_t* in, uint32_t length) const;

	// 8비트 배열을 32비트 배열로 변환한다.
	// - out: 출력 배열
	// - in: 입력 버퍼
	// - length: 변환할 길이
	void Size8ToSize32(OUT uint32_t* out, const uint8_t* in, uint32_t length) const;
};

#if 0
#include "Packet/Util/Md5.h"

std::string src("12345");
Md5 md5;
auto [c0, c1, c2, c3] = md5.GetChecksum((uint8_t*)src.data(), src.size() * sizeof(char));
std::string hex = md5.MakeHex();
hex;
//827ccb0eea8a706c4c34a16891f84e7b

#endif
