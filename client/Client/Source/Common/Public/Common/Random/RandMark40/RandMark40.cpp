// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Random/RandMark40/RandMark40.h"


RandMark40::RandMark40(uint64_t seed)
{
	mSeed = seed; // 시드 저장
	BuildReverseLookups();
	// mAllowedCvc가 비어있으므로 기본 금칙어 테이블을 구축
	BuildAllowedCvcTable({}); // 빈 금칙어 리스트로 기본 테이블 구축
}

void RandMark40::BuildAllowedCvcTable(const std::vector<std::wstring>& badWords)
{
	std::vector<std::wstring> bad;
	bad.reserve(badWords.size());
	for (const auto& word : badWords)
	{
		if (word.size() == 3)
			bad.push_back(ToLower(word));
	}
	
	// 금칙어를 set으로 정렬+unique로 처리(메모리 효율)
	std::sort(bad.begin(), bad.end());
	bad.erase(std::unique(bad.begin(), bad.end()), bad.end());

	mReverseCvc.fill(-1);
	mAllowedCvc.clear();
	mAllowedCvc.reserve(RandMark::MAX_CVC_COUNT);
	
	for (int c0 = 0; c0 < static_cast<int>(C.size()); ++c0)
	{
		for (int vv = 0; vv < static_cast<int>(V.size()); ++vv)
		{
			for (int c1 = 0; c1 < static_cast<int>(C.size()); ++c1)
			{
				uint16_t raw = RawIndexFromCvc(c0, vv, c1);
				std::wstring token;
				token.push_back(static_cast<wchar_t>(std::tolower(C[c0])));
				token.push_back(static_cast<wchar_t>(std::tolower(V[vv])));
				token.push_back(static_cast<wchar_t>(std::tolower(C[c1])));
				
				bool banned = std::binary_search(bad.begin(), bad.end(), token);
				if (!banned)
				{
					mReverseCvc[raw] = static_cast<int16_t>(mAllowedCvc.size());
					mAllowedCvc.push_back(raw);
				}
			}
		}
	}
	
	if (mAllowedCvc.empty())
		throw std::runtime_error("no allowed CVC");
}

// 40bit 인덱스를 4개 세그먼트로 변환 (CVC1, DDD2, CVC3, DDD4)
std::array<uint32_t, 4> RandMark40::IndexToSegments(uint64_t index) const
{
	std::array<uint32_t, 4> segments{};
	
	// 실제 구조: CVC1 * DDD2 * CVC3 * DDD4
	// CVC1: 0 ~ mAllowedCvc.size()-1
	// DDD2: 0 ~ 999
	// CVC3: 0 ~ mAllowedCvc.size()-1  
	// DDD4: 0 ~ 999
	
	uint64_t cvcCount = mAllowedCvc.size();
	uint64_t dddCount = 1000;
	
	// DDD4 추출
	segments[3] = static_cast<uint32_t>(index % dddCount);
	index /= dddCount;
	
	// CVC3 추출
	segments[2] = static_cast<uint32_t>(index % cvcCount);
	index /= cvcCount;
	
	// DDD2 추출
	segments[1] = static_cast<uint32_t>(index % dddCount);
	index /= dddCount;
	
	// CVC1 추출
	segments[0] = static_cast<uint32_t>(index % cvcCount);
	
	return segments;
}

// 4개 세그먼트를 인덱스로 변환 (CVC1, DDD2, CVC3, DDD4)
uint64_t RandMark40::SegmentsToIndex(const std::array<uint32_t, 4>& segments) const
{
	uint64_t index = 0;
	
	// 실제 구조: CVC1 * DDD2 * CVC3 * DDD4
	uint64_t cvcCount = mAllowedCvc.size();
	uint64_t dddCount = 1000;
	
	// CVC1 * DDD2 * CVC3 * DDD4 순서로 조합
	index = segments[0];                    // CVC1
	index = index * dddCount + segments[1]; // CVC1 * DDD2 + DDD2
	index = index * cvcCount + segments[2]; // (CVC1 * DDD2 + DDD2) * CVC3 + CVC3
	index = index * dddCount + segments[3]; // ((CVC1 * DDD2 + DDD2) * CVC3 + CVC3) * DDD4 + DDD4
	
	return index;
}

bool RandMark40::EncodeIndex(uint64_t index, std::wstring& result) const
{
	// 40bit 인덱스를 4개 10bit 세그먼트로 변환
	auto segments = IndexToSegments(index);
	
	// 각 세그먼트를 문자열로 변환
	std::wstring cvc1, ddd2, cvc3, ddd4;
	
	// CVC1: mAllowedCvc 인덱스를 raw CVC로 변환
	if (segments[0] >= mAllowedCvc.size())
		return false;
	if (!CvcFromRaw(mAllowedCvc[segments[0]], cvc1))
		return false;
	
	// DDD2
	if (!DddFromIndex(segments[1], ddd2))
		return false;
	
	// CVC3: mAllowedCvc 인덱스를 raw CVC로 변환
	if (segments[2] >= mAllowedCvc.size())
		return false;
	if (!CvcFromRaw(mAllowedCvc[segments[2]], cvc3))
		return false;
	
	// DDD4
	if (!DddFromIndex(segments[3], ddd4))
		return false;
	
	result = cvc1 + ddd2 + L"-" + cvc3 + ddd4;
	return true;
}

uint64_t RandMark40::DecodeIndex(const std::wstring& code) const
{
	// CVCDDD-CVCDDD 형식 파싱
	if (code.size() != 13 || code[6] != L'-')
		throw std::invalid_argument("invalid code format");
	
	// 각 부분 추출
	std::wstring cvc1 = code.substr(0, 3);   // CVC1
	std::wstring ddd2 = code.substr(3, 3);   // DDD2
	std::wstring cvc3 = code.substr(7, 3);   // CVC3
	std::wstring ddd4 = code.substr(10, 3);  // DDD4
	
	// 각 부분을 인덱스로 변환
	uint16_t raw1 = TryRawFromCvc(cvc1);
	int16_t acv1 = mReverseCvc[raw1];
	if (acv1 < 0)
		throw std::invalid_argument("CVC1 is banned");
	
	uint32_t idx2 = IndexFromDdd(ddd2);
	
	uint16_t raw3 = TryRawFromCvc(cvc3);
	int16_t acv3 = mReverseCvc[raw3];
	if (acv3 < 0)
		throw std::invalid_argument("CVC3 is banned");
	
	uint32_t idx4 = IndexFromDdd(ddd4);
	
	// 4개 세그먼트를 40bit 인덱스로 조합
	std::array<uint32_t, 4> segments = {
		static_cast<uint32_t>(acv1),  // CVC1
		idx2,                         // DDD2
		static_cast<uint32_t>(acv3),  // CVC3
		idx4                          // DDD4
	};
	
	return SegmentsToIndex(segments);
}

bool RandMark40::WasIssued(const std::wstring& code, uint64_t issuedCount) const
{
	uint64_t index = DecodeIndex(code);
	return index < issuedCount;
}

void RandMark40::BuildReverseLookups()
{
	mReverseC.fill(-1);
	mReverseV.fill(-1);
	for (int i = 0; i < static_cast<int>(C.size()); ++i)
		mReverseC[C[i] - L'A'] = i;
	for (int i = 0; i < static_cast<int>(V.size()); ++i)
		mReverseV[V[i] - L'A'] = i;
}

uint16_t RandMark40::RawIndexFromCvc(int c0, int vv, int c1) noexcept
{
	return static_cast<uint16_t>(c0) * (static_cast<uint16_t>(V.size()) * static_cast<uint16_t>(C.size()))
		+ static_cast<uint16_t>(vv) * static_cast<uint16_t>(C.size())
		+ static_cast<uint16_t>(c1);
}

bool RandMark40::CvcFromRaw(uint16_t raw, std::wstring& result)
{
	// 10bit CVC 인코딩 (0-1023)
	if (raw >= RandMark::MAX_CVC_COUNT)
		return false;
	
	// static 함수이므로 bad words 검사는 호출하는 쪽에서 처리
	// (EncodeIndex에서 이미 허용된 CVC만 사용)
	
	uint32_t c0 = static_cast<uint32_t>(raw / (V.size() * C.size()));
	uint32_t r = static_cast<uint32_t>(raw % (V.size() * C.size()));
	uint32_t vv = static_cast<uint32_t>(r / C.size());
	uint32_t c1 = static_cast<uint32_t>(r % C.size());
	
	result.resize(3);
	result[0] = C[c0];
	result[1] = V[vv];
	result[2] = C[c1];
	return true;
}

uint16_t RandMark40::TryRawFromCvc(const std::wstring& cvc) const
{
	if (cvc.size() != 3)
		throw std::invalid_argument("CVC len");
	
	std::wstring upperCvc = cvc;
	for (auto& ch : upperCvc)
		ch = static_cast<wchar_t>(std::toupper(ch));
	
	int c0 = (upperCvc[0] >= L'A' && upperCvc[0] <= L'Z') ? mReverseC[upperCvc[0] - L'A'] : -1;
	int vv = (upperCvc[1] >= L'A' && upperCvc[1] <= L'Z') ? mReverseV[upperCvc[1] - L'A'] : -1;
	int c1 = (upperCvc[2] >= L'A' && upperCvc[2] <= L'Z') ? mReverseC[upperCvc[2] - L'A'] : -1;
	
	if (c0 < 0 || vv < 0 || c1 < 0)
		throw std::invalid_argument("CVC letters");
	
	return RawIndexFromCvc(c0, vv, c1);
}

bool RandMark40::DddFromIndex(uint32_t index, std::wstring& result)
{
	// 10bit DDD 인코딩 (0-1023, 하지만 1000-1023은 무효)
	if (index >= 1024)  // 10bit 범위 초과
		return false;
	
	if (index >= 1000)  // DDD 유효 범위 초과 (0-999만 유효)
		return false;
	
	int value = static_cast<int>(index);
	result.resize(3);
	result[0] = static_cast<wchar_t>(L'0' + (value / 100) % 10);
	result[1] = static_cast<wchar_t>(L'0' + (value / 10) % 10);
	result[2] = static_cast<wchar_t>(L'0' + value % 10);
	return true;
}

uint32_t RandMark40::IndexFromDdd(const std::wstring& ddd)
{
	// 10bit DDD 디코딩 (000-999)
	if (ddd.size() != 3 || 
		ddd[0] < L'0' || ddd[0] > L'9' || 
		ddd[1] < L'0' || ddd[1] > L'9' || 
		ddd[2] < L'0' || ddd[2] > L'9')
		throw std::invalid_argument("invalid DDD format");
	
	int value = (ddd[0] - L'0') * 100 + (ddd[1] - L'0') * 10 + (ddd[2] - L'0');
	if (value < 0 || value >= static_cast<int>(DDD_COUNT))
		throw std::invalid_argument("DDD value out of range");
	
	return static_cast<uint32_t>(value);
}

std::wstring RandMark40::ToLower(const std::wstring& str) noexcept
{
	std::wstring result = str;
	for (auto& ch : result)
	{
		ch = static_cast<wchar_t>(std::tolower(ch));
	}
	return result;
}
