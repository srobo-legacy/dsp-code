#define SR_TEST_NAME "Test shr operation"
/* strtol saturates at max signed int, so we can't give value in raw hex */
#define SR_TEST_RET_VAL -100
#define SR_TEST_FILENAME shr.doff
#define SR_TEST_NUM_STR "00000007"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a >> 8;
}

int
test_main()
{

	return test(0xFFFF0000);
}

#endif
