#define SR_TEST_NAME "Test less-than-or-equal operation"
#define SR_TEST_RET_VAL 0x44
#define SR_TEST_FILENAME cmplte.doff
#define SR_TEST_NUM_STR "0000000F"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a <= 9) ? 17 : 0;
}

int
test_main()
{

	return test(8) + test(8) + test(9) + test(9) + test(10) + test(10);
}

#endif
