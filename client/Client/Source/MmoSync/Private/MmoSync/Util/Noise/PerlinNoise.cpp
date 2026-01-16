// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/Noise/PerlinNoise.h"


using namespace Noise;

constexpr uint8_t PerlinNoise::mPermutations[257] =
{
	151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
	140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
	247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
	 57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
	 74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
	 60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
	 65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
	200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
	 52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
	207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
	119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
	129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
	218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
	 81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
	184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
	222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
	151
};

uint16_t PerlinNoise::GetNoise(uint32_t x, uint32_t y)
{
	int32_t ans = NoiseImpl(x, y);
	ans = ans + 17308L;
	uint32_t pan = ans;
	pan *= 484L;
	return static_cast<uint16_t>(pan >> 8);
}

int16_t PerlinNoise::NoiseImpl(uint32_t x, uint32_t y)
{
	uint8_t X = static_cast<uint8_t>(x >> 16);
	uint8_t Y = static_cast<uint8_t>(y >> 16);

	uint8_t A = mPermutations[X] + Y;
	uint8_t AA = mPermutations[A];
	uint8_t AB = mPermutations[A + 1];
	uint8_t B = mPermutations[X + 1] + Y;
	uint8_t BA = mPermutations[B];
	uint8_t BB = mPermutations[B + 1];

	uint16_t u = x & 0xFFFF;
	uint16_t v = y & 0xFFFF;

	int16_t xx = (u >> 1) & 0x7FFF;
	int16_t yy = (v >> 1) & 0x7FFF;
	uint16_t N = 0x8000L;

	u = Ease(u);
	v = Ease(v);

	int16_t X1 = Lerp(Grad(mPermutations[AA], xx, yy), Grad(mPermutations[BA], xx - N, yy), u);
	int16_t X2 = Lerp(Grad(mPermutations[AB], xx, yy - N), Grad(mPermutations[BB], xx - N, yy - N), u);

	int16_t ans = Lerp(X1, X2, v);

	return ans;
}

uint16_t PerlinNoise::Scale(uint16_t i, uint16_t scale)
{
	return static_cast<uint16_t>(((uint32_t)(i) * (uint32_t)(scale)) / 65536);
}

uint16_t PerlinNoise::Ease(uint16_t i)
{
	uint16_t j = i;
	if (j & 0x8000) {
		j = 65535 - j;
	}
	uint16_t jj = Scale(j, j);
	uint16_t jj2 = jj << 1;
	if (i & 0x8000) {
		jj2 = 65535 - jj2;
	}
	return jj2;
}

int16_t PerlinNoise::Grad(uint8_t hash, int16_t x, int16_t y)
{
	hash = hash & 7;
	int16_t u, v;
	if (hash < 4) { u = x; v = y; }
	else { u = y; v = x; }
	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return (u + v) >> 1;
}

int16_t PerlinNoise::Lerp(int16_t a, int16_t b, uint16_t frac)
{
	int16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = Scale(delta, frac);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = Scale(delta, frac);
		result = a - scaled;
	}
	return result;
}
