#include "all.h"

int32_t
basic(const uint8_t *input, size_t length)
{
	int32_t res = 0;

	while (true) {
		uint8_t c = *input;
		input++;
		switch (c) {
		case '\0':
			return res;
		case 's':
			res++;
			break;
		case 'p':
			res--;
			break;
		default:
			break;
		}
	}
}
