#define SR_TEST_NAME "Test greater-than-or-equal operation"
#define SR_TEST_RET_VAL 0xC
#define SR_TEST_FILENAME cmpgte.doff
#define SR_TEST_NUM_STR "00000010"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a >= 6) ? 3 : 0;
}

int
test_main()
{

	return test(5) + test(5) + test(6) + test(6) + test(7) + test(7);
}

#endif
