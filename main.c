#include "all.h"

static int32_t
compare_uint64(const void *x_ptr, const void *y_ptr)
{
	uint64_t x = *((uint64_t *)x_ptr);
	uint64_t y = *((uint64_t *)y_ptr);

	if (x == y) {
		return 0;
	}

	if (x > y) {
		return 1;
	}

	return -1;
}

static uint64_t
system_time(void)
{
	return clock_gettime_nsec_np(CLOCK_MONOTONIC);
}

static uint8_t *
gen_input(size_t size)
{
	assert(size % CHAR_BIT == 0);

	uint8_t *random = malloc(size / CHAR_BIT);
	uint8_t *input = malloc(size + 1);

	arc4random_buf(random, size / CHAR_BIT);

	for (size_t i = 0; i < size; i++) {
		uint8_t bit = random[i / CHAR_BIT] & (1 << (i % CHAR_BIT));
		input[i] = bit == 0 ? 's' : 'p';
	}

	input[size] = '\0';
	free(random);
	return input;
}

static uint64_t
estimate_impl_duration(
        struct implementation impl, const uint8_t *input, size_t length)
{
	uint64_t iters = 100;
	uint64_t before = system_time();
	for (uint64_t i = 0; i < iters; i++) {
		impl.fn(input, length);
	}
	uint64_t after = system_time();

	return (after - before) / iters;
}

static void
run(struct implementation impl, const uint8_t *input, size_t length)
{
	printf("%s:\n", impl.name);

	uint64_t desired_total_duration = 5ll * 1000 * 1000 * 1000; // 5 seconds

	uint64_t estimated_duration =
	        estimate_impl_duration(impl, input, length);

	// We group runs into chunks
	// to avoid checking the system time too often.
	uint64_t chunk_size = 128;

	uint64_t iter_count =
	        desired_total_duration / estimated_duration / chunk_size;
	printf("%20llu iters\n", iter_count);

	uint64_t *durations = calloc(iter_count, sizeof(uint64_t));

	for (uint64_t i = 0; i < iter_count; i++) {
		uint64_t before = system_time();
		for (uint64_t j = 0; j < chunk_size; j++) {
			impl.fn(input, length);
		}
		uint64_t after = system_time();

		durations[i] = (after - before) / chunk_size;
	}

	qsort(durations, iter_count, sizeof(uint64_t), compare_uint64);
	uint64_t median_duration = durations[iter_count / 2];
	free(durations);

	printf("%20.3f ns/b\n", (double)median_duration / (double)length);

	// b/ns == GB/s
	double throughput = (double)length / (double)median_duration;
	printf("%20.3f GB/s\n", throughput);
}

int32_t
main(void)
{
	// For fairness:
	// https://github.com/414owen/blog-code/blob/c6c0d5f73c02da202c1ffc257598c5be2b1198b9/01-six-times-faster-than-c/bench.c#L9
	size_t length = 1000000;
	uint8_t *input = gen_input(length);

	int32_t expected_result = basic(input, length);
	struct implementation *impl = implementations;

	while (impl->fn != NULL) {
		int32_t actual_result = impl->fn(input, length);
		if (actual_result != expected_result) {
			printf("%s failed: expected %d, got %d.\n", impl->name,
			        expected_result, actual_result);
			return 1;
		}
		impl++;
	}

	impl = implementations;

	while (impl->fn != NULL) {
		run(*impl, input, length);
		impl++;
	}

	free(input);
}
