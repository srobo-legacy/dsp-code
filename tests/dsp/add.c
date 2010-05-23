#define SR_TEST_NAME "Test add operation"
#define SR_TEST_RET_VAL 0x3A3A
#define SR_TEST_FILENAME add.doff
#define SR_TEST_NUM 4
#define SR_TEST_NUM_STR "00000004"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a, int b)
{

	return a + b;
}

int
test_main()
{

	return test(0x3A00, 0x3A);
}

#endif
