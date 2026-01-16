// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <utility>

#include "Common/Random/RandMark40/RandMarkDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RandMark40 - 40비트 고유 코드 생성기 (10bit × 4개 순서)
// 
// 구조: CVCDDD-CVCDDD
// - 1번째 CVC: 10bit (16×4×16 = 1024가지, bad words 제외)
// - 2번째 DDD: 10bit (0-999, 1000가지)
// - 3번째 CVC: 10bit (16×4×16 = 1024가지, bad words 제외)
// - 4번째 DDD: 10bit (0-999, 1000가지)
// - 전체: 40bit (10bit × 4개)
// 
// 용량: 허용CVC × 1000 × 허용CVC × 1000
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class COMMON_API RandMark40 final
{
private:
	// 40bit 구조 상수 (10bit × 4개 순서)
	static constexpr auto& C = RandMark::CONSONANTS;
	static constexpr auto& V = RandMark::VOWELS;
	static constexpr uint32_t DDD_COUNT = RandMark::DDD_COUNT;        // 1000 (10bit)
	static constexpr uint32_t MAX_CVC_COUNT = RandMark::MAX_CVC_COUNT; // 1024 (10bit)
	static constexpr size_t TOTAL_BITS = RandMark::TOTAL_BITS;        // 40bit
	static constexpr size_t BITS_PER_SEGMENT = RandMark::BITS_PER_SEGMENT; // 10bit

	// 10bit CVC 테이블 (1024가지 중 bad words 제외)
	std::vector<uint16_t> mAllowedCvc;        // 허용 raw CVC 인덱스(0..1023)
	std::array<int16_t, 1024> mReverseCvc{};  // raw->allowed index (banned:-1)
	
	// 룩업 테이블
	std::array<int, 26> mReverseC{};          // A..Z -> 자음 인덱스(or -1)
	std::array<int, 26> mReverseV{};          // A..Z -> 모음 인덱스(or -1)
	
	// 40bit → 4개 10bit 변환을 위한 상수
	uint64_t mSeed{}; // 생성자에서 받은 시드 저장

public:
	// 생성자
	explicit RandMark40(uint64_t seed = 0x9e3779b97f4a7c15ULL);
	
	// 금칙어 목록으로 허용 CVC 테이블 구성
	void BuildAllowedCvcTable(const std::vector<std::wstring>& badWords);

	// 40bit 인덱스를 4개 10bit 세그먼트로 변환
	std::array<uint32_t, 4> IndexToSegments(uint64_t index) const;
	
	// 4개 10bit 세그먼트를 40bit 인덱스로 변환
	uint64_t SegmentsToIndex(const std::array<uint32_t, 4>& segments) const;

	// 40bit 인덱스 → 코드("CVCDDD-CVCDDD")
	bool EncodeIndex(uint64_t index, std::wstring& result) const;
	
	// 코드 → 40bit 인덱스
	uint64_t DecodeIndex(const std::wstring& code) const;
	
	// 발급 여부 판별(issuedCount: 현재까지 발급한 개수)
	bool WasIssued(const std::wstring& code, uint64_t issuedCount) const;

	// 허용된 CVC 개수 반환 (테스트용)
	size_t GetAllowedCvcCount() const { return mAllowedCvc.size(); }

	// CVC 원-인덱스 변환(0..1023) - 테스트용 public
	static bool CvcFromRaw(uint16_t raw, std::wstring& result);
	
	// DDD 인덱스 변환(0..999) - 테스트용 public
	static bool DddFromIndex(uint32_t index, std::wstring& result);

private:
	// 룩업 테이블 구성
	void BuildReverseLookups();
	
	// CVC 원-인덱스 변환(0..1023)
	static uint16_t RawIndexFromCvc(int c0, int vv, int c1) noexcept;
	uint16_t TryRawFromCvc(const std::wstring& cvc) const;
	
	// DDD 인덱스 변환(0..999)
	static uint32_t IndexFromDdd(const std::wstring& ddd);
	
	// 유틸리티
	static std::wstring ToLower(const std::wstring& str) noexcept;
};
