#define SR_TEST_NAME "Test shr operation"
#define SR_TEST_RET_VAL 7
#define SR_TEST_FILENAME shr.doff
#define SR_TEST_NUM 7
#define SR_TEST_NUM_STR "00000007"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a >> 12;
}

int
test_main()
{

	return test(0x7554);
}

#endif
