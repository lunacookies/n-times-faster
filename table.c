#include "all.h"

static int32_t to_add[256] = {
	['s'] = 1,
	['p'] = -1,
};

int32_t
table(const uint8_t *input, size_t length)
{
	int32_t res = 0;

	while (true) {
		uint8_t c = *input;
		input++;
		if (c == '\0') {
			return res;
		}
		res += to_add[c];
	}
}
