// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


namespace Noise
{
	class MMOSYNC_API PerlinNoise
	{
	private:
		static uint8_t const mPermutations[257];

	public:
		static uint16_t GetNoise(uint32_t x, uint32_t y);

	private:
		static int16_t NoiseImpl(uint32_t x, uint32_t y);
		static uint16_t Scale(uint16_t i, uint16_t scale);
		static uint16_t Ease(uint16_t i);
		static int16_t Grad(uint8_t hash, int16_t x, int16_t y);
		static int16_t Lerp(int16_t a, int16_t b, uint16_t frac);
	};
}
