#include <arm_neon.h>
#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int32_t (*implementation_fn)(const uint8_t *input, size_t length);

struct implementation {
	implementation_fn fn;
	const char *name;
};

int32_t basic(const uint8_t *input, size_t length);

static struct implementation implementations[] = {
	{ .fn = basic, .name = "basic" },
	{ 0 },
};
