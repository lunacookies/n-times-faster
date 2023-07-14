#include "all.h"

static int32_t to_add[256] = {
	['s'] = 1,
	['p'] = -1,
};

int32_t
table_16(const uint8_t *input, size_t length)
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
	const __uint128_t *chunks = (const __uint128_t *)input;
	const __uint128_t *chunks_end = chunks + chunk_count;

	while (chunks != chunks_end) {
		__uint128_t chunk = *chunks;
		chunks++;

		uint8_t bytes[16];
		memcpy(bytes, &chunk, 16);

		for (size_t i = 0; i < 16; i++) {
			res += to_add[bytes[i]];
		}
	}

	input = (const uint8_t *)chunks;

	res += table_length(input, slack);

	return res;
}
