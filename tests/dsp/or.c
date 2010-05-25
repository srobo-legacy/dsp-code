#define SR_TEST_NAME "Test or operation"
#define SR_TEST_RET_VAL 0xFFFF
#define SR_TEST_FILENAME or.doff
#define SR_TEST_NUM_STR "0000000B"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a | 0x3333;
}

int
test_main()
{

	return test(0xCCCC);
}

#endif
