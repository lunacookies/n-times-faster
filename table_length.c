#include "all.h"

static int32_t to_add[256] = {
	['s'] = 1,
	['p'] = -1,
};

int32_t
table_length(const uint8_t *input, size_t length)
{
	int32_t res = 0;
	const uint8_t *end = input + length;

	while (input != end) {
		uint8_t c = *input;
		input++;
		res += to_add[c];
	}

	return res;
}
