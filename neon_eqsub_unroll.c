#include "all.h"

int32_t
neon_eqsub_unroll(const uint8_t *input, size_t length)
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

	uint8x16_t s = vmovq_n_u8('s');
	uint8x16_t p = vmovq_n_u8('p');

	while (chunks != chunks_end) {
		uint8x16_t acc = vmovq_n_u8(0);

		for (uint32_t j = 0; j < 8; j++) {
			uint8x16_t chunk = *chunks;
			chunks++;

			uint8x16_t matched_s = vceqq_u8(chunk, s);
			uint8x16_t matched_p = vceqq_u8(chunk, p);
			uint8x16_t matched = vsubq_u8(matched_p, matched_s);

			acc = vaddq_u8(acc, matched);
		}

		res += vaddlvq_s8(acc);
	}

	input = (const uint8_t *)chunks;

	res += table_length(input, slack);

	return res;
}
