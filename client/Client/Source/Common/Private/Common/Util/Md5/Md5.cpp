// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Util/Md5/Md5.h"


Md5::Md5()
{
	Clear();
}
void Md5::Clear()
{
	// [WHY] MD5 초기 상태 값 (RFC 1321 표준)
	constexpr uint32_t MD5_INIT_STATE_0 = 0x67452301;
	constexpr uint32_t MD5_INIT_STATE_1 = 0xefcdab89;
	constexpr uint32_t MD5_INIT_STATE_2 = 0x98badcfe;
	constexpr uint32_t MD5_INIT_STATE_3 = 0x10325476;

	mBuffer.fill(0);
	mCount.fill(0);

	mChecksum[0] = MD5_INIT_STATE_0;
	mChecksum[1] = MD5_INIT_STATE_1;
	mChecksum[2] = MD5_INIT_STATE_2;
	mChecksum[3] = MD5_INIT_STATE_3;
}

const std::array<uint32_t, 4>& Md5::GetChecksum(const uint8_t* buffer, size_t length)
{
	Clear();

	Update(buffer, static_cast<uint32_t>(length));

	Final();

	return mChecksum;
}

std::string Md5::MakeHexStr() const
{
	constexpr char DIGITS[] = "0123456789ABCDEF";

	uint8_t bytes[16];
	Size32ToSize8(bytes, mChecksum.data(), 16);

	std::string result;
	for (size_t i = 0; i < 16; ++i) 
	{
		result += DIGITS[bytes[i] >> 4];
		result += DIGITS[bytes[i] & 15];
	}	
	return result;
}

std::wstring Md5::MakeHexStrW() const
{
	constexpr wchar_t DIGITS[] = L"0123456789ABCDEF";

	uint8_t bytes[16];
	Size32ToSize8(bytes, mChecksum.data(), 16);

	std::wstring result;
	for (size_t i = 0; i < 16; ++i)
	{
		result += DIGITS[bytes[i] >> 4];
		result += DIGITS[bytes[i] & 15];
	}
	return result;
}

void Md5::Final()
{
	constexpr uint8_t PADDING[64] =
	{
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	uint8_t bytes[8];
	Size32ToSize8(bytes, mCount.data(), 8);

	const uint32_t index = static_cast<uint32_t>((mCount[0] >> 3) & 0x3f);
	const uint32_t length = (index < 56) ? (56 - index) : (120 - index);

	Update(PADDING, length);
	Update(bytes, 8);
}

void Md5::Update(const uint8_t* in, uint32_t length)
{
	if ((nullptr == in) || (length == 0))
		return;

	uint32_t index = static_cast<uint32_t>((mCount[0] >> 3) & 0x3f);

	if ((mCount[0] += length << 3) < (length << 3))
		++mCount[1];

	mCount[1] += (length >> 29);

	uint32_t i = 0;
	const uint32_t partLength = 64 - index;
	if (length >= partLength)
	{
		std::copy(&in[0], &in[partLength], mBuffer.begin() + index);
		Transform(mBuffer.data());
		for (i = partLength; i + 63 < length; i += 64)
		{
			Transform(&in[i]);
		}
		index = 0;
	}

	std::copy(&in[i], &in[length], mBuffer.begin() + index);
}

void Md5::Transform(const uint8_t* block)
{
	constexpr uint32_t MD5_S11 = 7;
	constexpr uint32_t MD5_S12 = 12;
	constexpr uint32_t MD5_S13 = 17;
	constexpr uint32_t MD5_S14 = 22;
	constexpr uint32_t MD5_S21 = 5;
	constexpr uint32_t MD5_S22 = 9;
	constexpr uint32_t MD5_S23 = 14;
	constexpr uint32_t MD5_S24 = 20;
	constexpr uint32_t MD5_S31 = 4;
	constexpr uint32_t MD5_S32 = 11;
	constexpr uint32_t MD5_S33 = 16;
	constexpr uint32_t MD5_S34 = 23;
	constexpr uint32_t MD5_S41 = 6;
	constexpr uint32_t MD5_S42 = 10;
	constexpr uint32_t MD5_S43 = 15;
	constexpr uint32_t MD5_S44 = 21;

	constexpr uint32_t MD5_T01 = 0xd76aa478;
	constexpr uint32_t MD5_T02 = 0xe8c7b756;
	constexpr uint32_t MD5_T03 = 0x242070db;
	constexpr uint32_t MD5_T04 = 0xc1bdceee;
	constexpr uint32_t MD5_T05 = 0xf57c0faf;
	constexpr uint32_t MD5_T06 = 0x4787c62a;
	constexpr uint32_t MD5_T07 = 0xa8304613;
	constexpr uint32_t MD5_T08 = 0xfd469501;
	constexpr uint32_t MD5_T09 = 0x698098d8;
	constexpr uint32_t MD5_T10 = 0x8b44f7af;
	constexpr uint32_t MD5_T11 = 0xffff5bb1;
	constexpr uint32_t MD5_T12 = 0x895cd7be;
	constexpr uint32_t MD5_T13 = 0x6b901122;
	constexpr uint32_t MD5_T14 = 0xfd987193;
	constexpr uint32_t MD5_T15 = 0xa679438e;
	constexpr uint32_t MD5_T16 = 0x49b40821;

	constexpr uint32_t MD5_T17 = 0xf61e2562;
	constexpr uint32_t MD5_T18 = 0xc040b340;
	constexpr uint32_t MD5_T19 = 0x265e5a51;
	constexpr uint32_t MD5_T20 = 0xe9b6c7aa;
	constexpr uint32_t MD5_T21 = 0xd62f105d;
	constexpr uint32_t MD5_T22 = 0x02441453;
	constexpr uint32_t MD5_T23 = 0xd8a1e681;
	constexpr uint32_t MD5_T24 = 0xe7d3fbc8;
	constexpr uint32_t MD5_T25 = 0x21e1cde6;
	constexpr uint32_t MD5_T26 = 0xc33707d6;
	constexpr uint32_t MD5_T27 = 0xf4d50d87;
	constexpr uint32_t MD5_T28 = 0x455a14ed;
	constexpr uint32_t MD5_T29 = 0xa9e3e905;
	constexpr uint32_t MD5_T30 = 0xfcefa3f8;
	constexpr uint32_t MD5_T31 = 0x676f02d9;
	constexpr uint32_t MD5_T32 = 0x8d2a4c8a;
 
	constexpr uint32_t MD5_T33 = 0xfffa3942;
	constexpr uint32_t MD5_T34 = 0x8771f681;
	constexpr uint32_t MD5_T35 = 0x6d9d6122;
	constexpr uint32_t MD5_T36 = 0xfde5380c;
	constexpr uint32_t MD5_T37 = 0xa4beea44;
	constexpr uint32_t MD5_T38 = 0x4bdecfa9;
	constexpr uint32_t MD5_T39 = 0xf6bb4b60;
	constexpr uint32_t MD5_T40 = 0xbebfbc70;
	constexpr uint32_t MD5_T41 = 0x289b7ec6;
	constexpr uint32_t MD5_T42 = 0xeaa127fa;
	constexpr uint32_t MD5_T43 = 0xd4ef3085;
	constexpr uint32_t MD5_T44 = 0x04881d05;
	constexpr uint32_t MD5_T45 = 0xd9d4d039;
	constexpr uint32_t MD5_T46 = 0xe6db99e5;
	constexpr uint32_t MD5_T47 = 0x1fa27cf8;
	constexpr uint32_t MD5_T48 = 0xc4ac5665;

	constexpr uint32_t MD5_T49 = 0xf4292244;
	constexpr uint32_t MD5_T50 = 0x432aff97;
	constexpr uint32_t MD5_T51 = 0xab9423a7;
	constexpr uint32_t MD5_T52 = 0xfc93a039;
	constexpr uint32_t MD5_T53 = 0x655b59c3;
	constexpr uint32_t MD5_T54 = 0x8f0ccc92;
	constexpr uint32_t MD5_T55 = 0xffeff47d;
	constexpr uint32_t MD5_T56 = 0x85845dd1;
	constexpr uint32_t MD5_T57 = 0x6fa87e4f;
	constexpr uint32_t MD5_T58 = 0xfe2ce6e0;
	constexpr uint32_t MD5_T59 = 0xa3014314;
	constexpr uint32_t MD5_T60 = 0x4e0811a1;
	constexpr uint32_t MD5_T61 = 0xf7537e82;
	constexpr uint32_t MD5_T62 = 0xbd3af235;
	constexpr uint32_t MD5_T63 = 0x2ad7d2bb;
	constexpr uint32_t MD5_T64 = 0xeb86d391;

	uint32_t a = mChecksum[0];
	uint32_t b = mChecksum[1];
	uint32_t c = mChecksum[2];
	uint32_t d = mChecksum[3];

	uint32_t X[16];
	Size8ToSize32(X, block, 64);

	FF(a, b, c, d, X[0], MD5_S11, MD5_T01);
	FF(d, a, b, c, X[1], MD5_S12, MD5_T02);
	FF(c, d, a, b, X[2], MD5_S13, MD5_T03);
	FF(b, c, d, a, X[3], MD5_S14, MD5_T04);
	FF(a, b, c, d, X[4], MD5_S11, MD5_T05);
	FF(d, a, b, c, X[5], MD5_S12, MD5_T06);
	FF(c, d, a, b, X[6], MD5_S13, MD5_T07);
	FF(b, c, d, a, X[7], MD5_S14, MD5_T08);
	FF(a, b, c, d, X[8], MD5_S11, MD5_T09);
	FF(d, a, b, c, X[9], MD5_S12, MD5_T10);
	FF(c, d, a, b, X[10], MD5_S13, MD5_T11);
	FF(b, c, d, a, X[11], MD5_S14, MD5_T12);
	FF(a, b, c, d, X[12], MD5_S11, MD5_T13);
	FF(d, a, b, c, X[13], MD5_S12, MD5_T14);
	FF(c, d, a, b, X[14], MD5_S13, MD5_T15);
	FF(b, c, d, a, X[15], MD5_S14, MD5_T16);

	GG(a, b, c, d, X[1], MD5_S21, MD5_T17);
	GG(d, a, b, c, X[6], MD5_S22, MD5_T18);
	GG(c, d, a, b, X[11], MD5_S23, MD5_T19);
	GG(b, c, d, a, X[0], MD5_S24, MD5_T20);
	GG(a, b, c, d, X[5], MD5_S21, MD5_T21);
	GG(d, a, b, c, X[10], MD5_S22, MD5_T22);
	GG(c, d, a, b, X[15], MD5_S23, MD5_T23);
	GG(b, c, d, a, X[4], MD5_S24, MD5_T24);
	GG(a, b, c, d, X[9], MD5_S21, MD5_T25);
	GG(d, a, b, c, X[14], MD5_S22, MD5_T26);
	GG(c, d, a, b, X[3], MD5_S23, MD5_T27);
	GG(b, c, d, a, X[8], MD5_S24, MD5_T28);
	GG(a, b, c, d, X[13], MD5_S21, MD5_T29);
	GG(d, a, b, c, X[2], MD5_S22, MD5_T30);
	GG(c, d, a, b, X[7], MD5_S23, MD5_T31);
	GG(b, c, d, a, X[12], MD5_S24, MD5_T32);

	HH(a, b, c, d, X[5], MD5_S31, MD5_T33);
	HH(d, a, b, c, X[8], MD5_S32, MD5_T34);
	HH(c, d, a, b, X[11], MD5_S33, MD5_T35);
	HH(b, c, d, a, X[14], MD5_S34, MD5_T36);
	HH(a, b, c, d, X[1], MD5_S31, MD5_T37);
	HH(d, a, b, c, X[4], MD5_S32, MD5_T38);
	HH(c, d, a, b, X[7], MD5_S33, MD5_T39);
	HH(b, c, d, a, X[10], MD5_S34, MD5_T40);
	HH(a, b, c, d, X[13], MD5_S31, MD5_T41);
	HH(d, a, b, c, X[0], MD5_S32, MD5_T42);
	HH(c, d, a, b, X[3], MD5_S33, MD5_T43);
	HH(b, c, d, a, X[6], MD5_S34, MD5_T44);
	HH(a, b, c, d, X[9], MD5_S31, MD5_T45);
	HH(d, a, b, c, X[12], MD5_S32, MD5_T46);
	HH(c, d, a, b, X[15], MD5_S33, MD5_T47);
	HH(b, c, d, a, X[2], MD5_S34, MD5_T48);

	II(a, b, c, d, X[0], MD5_S41, MD5_T49);
	II(d, a, b, c, X[7], MD5_S42, MD5_T50);
	II(c, d, a, b, X[14], MD5_S43, MD5_T51);
	II(b, c, d, a, X[5], MD5_S44, MD5_T52);
	II(a, b, c, d, X[12], MD5_S41, MD5_T53);
	II(d, a, b, c, X[3], MD5_S42, MD5_T54);
	II(c, d, a, b, X[10], MD5_S43, MD5_T55);
	II(b, c, d, a, X[1], MD5_S44, MD5_T56);
	II(a, b, c, d, X[8], MD5_S41, MD5_T57);
	II(d, a, b, c, X[15], MD5_S42, MD5_T58);
	II(c, d, a, b, X[6], MD5_S43, MD5_T59);
	II(b, c, d, a, X[13], MD5_S44, MD5_T60);
	II(a, b, c, d, X[4], MD5_S41, MD5_T61);
	II(d, a, b, c, X[11], MD5_S42, MD5_T62);
	II(c, d, a, b, X[2], MD5_S43, MD5_T63);
	II(b, c, d, a, X[9], MD5_S44, MD5_T64);

	mChecksum[0] += a;
	mChecksum[1] += b;
	mChecksum[2] += c;
	mChecksum[3] += d;
}

uint32_t Md5::RotateLeft(uint32_t x, int32_t n) const
{
	return (x << n) | (x >> (32 - n));
}

void Md5::FF(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const
{
	const uint32_t F = (B & C) | (~B & D);
	A += F + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void Md5::GG(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const
{
	const uint32_t G = (B & D) | (C & ~D);
	A += G + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void Md5::HH(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const
{
	uint32_t H = (B ^ C ^ D);
	A += H + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void Md5::II(IN OUT uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T) const
{
	const uint32_t I = (C ^ (B | ~D));
	A += I + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void Md5::Size32ToSize8(OUT uint8_t* out, const uint32_t* in, uint32_t length) const
{
	size_t i = 0;
	for (size_t j = 0; j < length; j += 4)
	{
		out[j] = static_cast<uint8_t>(in[i] & 0xff);
		out[j + 1] = static_cast<uint8_t>((in[i] >> 8) & 0xff);
		out[j + 2] = static_cast<uint8_t>((in[i] >> 16) & 0xff);
		out[j + 3] = static_cast<uint8_t>((in[i] >> 24) & 0xff);

		++i;
	}
}

void Md5::Size8ToSize32(OUT uint32_t* out, const uint8_t* in, uint32_t length) const
{
	size_t i = 0;
	for (size_t j = 0; j < length; j += 4)
	{
		out[i] =
			static_cast<uint32_t>(in[j]) |
			static_cast<uint32_t>(in[j + 1]) << 8 |
			static_cast<uint32_t>(in[j + 2]) << 16 |
			static_cast<uint32_t>(in[j + 3]) << 24;

		++i;
	}
}
