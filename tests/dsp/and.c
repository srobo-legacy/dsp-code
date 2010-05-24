#define SR_TEST_NAME "Test and operation"
#define SR_TEST_RET_VAL 0x3333
#define SR_TEST_FILENAME and.doff
#define SR_TEST_NUM A
#define SR_TEST_NUM_STR "0000000A"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a & 0x3333;
}

int
test_main()
{

	return test(0xFFFF);
}

#endif
