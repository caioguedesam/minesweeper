#pragma once
#include <stdint.h>
#include <time.h>

// Pseudo-number generation using Xor-Shift
static inline uint64_t xor_shift_64()
{
	static uint64_t seed = time(NULL);
	seed ^= seed >> 12;
	seed ^= seed << 25;
	seed ^= seed >> 27;
	return seed * 0x2545F4914F6CDD1DULL;
}

// Returns a sample from an uniform distribution between a start and an end
static inline float dist_uniform(const float& start = 0.f, const float& end = 1.f)
{
	return start + (static_cast<float>(xor_shift_64()) / static_cast<float>(UINT64_MAX)) * (end - start);
}
