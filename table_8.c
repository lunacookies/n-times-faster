#include "all.h"

static int32_t to_add[256] = {
	['s'] = 1,
	['p'] = -1,
};

int32_t
table_8(const uint8_t *input, size_t length)
{
	size_t padding = 0;
	if ((uintptr_t)input % 8 != 0) {
		padding = 8 - (uintptr_t)input % 8;
	}

	int32_t res = table_length(input, padding);
	input += padding;
	length -= padding;

	size_t chunk_count = length / 8;
	size_t slack = length % 8;
	const uint64_t *chunks = (const uint64_t *)input;
	const uint64_t *chunks_end = chunks + chunk_count;

	while (chunks != chunks_end) {
		uint64_t chunk = *chunks;
		chunks++;

		uint8_t bytes[8];
		memcpy(bytes, &chunk, 8);

		for (size_t i = 0; i < 8; i++) {
			res += to_add[bytes[i]];
		}
	}

	input = (const uint8_t *)chunks;

	res += table_length(input, slack);

	return res;
}
