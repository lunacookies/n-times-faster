#include "all.h"

int32_t
neon_lsb_unroll(const uint8_t *input, size_t length)
{
	size_t padding = 0;
	if ((uintptr_t)input % 16 != 0) {
		padding = 16 - (uintptr_t)input % 16;
	}

	int32_t res = table_length(input, padding);
	input += padding;
	length -= padding;

	size_t iter_count = length / (16 * 8);
	size_t slack = length % (16 * 8);
	size_t chunk_count = iter_count * 8;
	const uint8x16_t *chunks = (const uint8x16_t *)input;
	const uint8x16_t *chunks_end = chunks + chunk_count;

	uint8x16_t ones = vmovq_n_u8(1);

	while (chunks != chunks_end) {
		uint8x16_t acc = vmovq_n_u8(0);

		for (uint32_t j = 0; j < 8; j++) {
			uint8x16_t chunk = *chunks;
			chunks++;
			acc = vaddq_u8(acc, vandq_u8(chunk, ones));
		}

		res += 2 * vaddlvq_s8(acc) - 16 * 8;
	}

	input = (const uint8_t *)chunks;

	res += table_length(input, slack);

	return res;
}
