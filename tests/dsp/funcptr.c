#define SR_TEST_NAME "indirect function pointer call"
#define SR_TEST_RET_VAL 0xCC3
#define SR_TEST_FILENAME funcptr.doff
#define SR_TEST_NUM_STR "0000001A"

#include "sr_test.h"

#ifndef GENERATE_XML

#include <stdint.h>

#include <sr_hacks.h>

uint32_t
test(uint32_t a)
{

	return a;
}

int
test_main()
{
	uint32_t (*func)(uint32_t a);

	func = test;

	return func(0xCC3);
}

#endif
