#define SR_TEST_NAME "Test greater-than operation"
#define SR_TEST_RET_VAL 0x14
#define SR_TEST_FILENAME cmpgt.doff
#define SR_TEST_NUM_STR "0000000E"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a > 9) ? 10 : 0;
}

int
test_main()
{

	return test(8) + test(8) + test(9) + test(9) + test(10) + test(10);
}

#endif
