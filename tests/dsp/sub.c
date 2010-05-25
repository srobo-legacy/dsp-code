#define SR_TEST_NAME "Test sub operation"
#define SR_TEST_RET_VAL 0x20
#define SR_TEST_FILENAME sub.doff
#define SR_TEST_NUM_STR "00000005"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a, int b)
{

	return a - b;
}

int
test_main()
{

	return test(0x40, 0x20);
}

#endif
