#include "all.h"

int32_t
neon(const uint8_t *input, size_t length)
{
	size_t padding = 0;
	if ((uintptr_t)input % 16 != 0) {
		padding = 16 - (uintptr_t)input % 16;
	}

	int32_t res = table_length(input, padding);
	input += padding;
	length -= padding;

	size_t chunk_count = length / 16;
	size_t slack = length % 16;
	const uint8x16_t *chunks = (const uint8x16_t *)input;
	const uint8x16_t *chunks_end = chunks + chunk_count;

	uint8x16_t s = vmovq_n_u8('s');
	uint8x16_t p = vmovq_n_u8('p');
	uint8x16_t ones = vmovq_n_u8(1);

	while (chunks != chunks_end) {
		uint8x16_t chunk = *chunks;
		chunks++;

		uint8x16_t matched_s = vandq_u8(vceqq_u8(chunk, s), ones);
		uint8x16_t matched_p = vceqq_u8(chunk, p);
		uint8x16_t matched = vorrq_u8(matched_s, matched_p);

		res += vaddlvq_s8(matched);
	}

	input = (const uint8_t *)chunks;

	res += table_length(input, slack);

	return res;
}
