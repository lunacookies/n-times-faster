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
int32_t table(const uint8_t *input, size_t length);
int32_t table_length(const uint8_t *input, size_t length);
int32_t table_8(const uint8_t *input, size_t length);
int32_t table_16(const uint8_t *input, size_t length);
int32_t neon(const uint8_t *input, size_t length);
int32_t neon_less_reduce(const uint8_t *input, size_t length);
int32_t neon_lsb(const uint8_t *input, size_t length);
int32_t eqsub(const uint8_t *input, size_t length);

static struct implementation implementations[] = {
	{ .fn = basic, .name = "basic" },
	{ .fn = table, .name = "table" },
	{ .fn = table_length, .name = "table_length" },
	{ .fn = table_8, .name = "table_8" },
	{ .fn = table_16, .name = "table_16" },
	{ .fn = neon, .name = "neon" },
	{ .fn = neon_less_reduce, .name = "neon_less_reduce" },
	{ .fn = neon_lsb, .name = "neon_lsb" },
	{ .fn = eqsub, .name = "eqsub" },
	{ 0 },
};
