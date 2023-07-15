#include "all.h"

int32_t
lsb(const uint8_t *input, size_t length)
{
	int32_t res = 0;
	size_t chunk_size = 64;
	size_t iters = length / chunk_size;
	size_t slack = length % chunk_size;

	for (size_t i = 0; i < iters; i++) {
		int8_t acc = 0;
		for (size_t j = 0; j < chunk_size; j++) {
			acc += *input & 1;
			input++;
		}
		res += acc;
	}

	for (size_t i = 0; i < slack; i++) {
		res += *input & 1;
		input++;
	}

	return 2 * res - (int32_t)length;
}
