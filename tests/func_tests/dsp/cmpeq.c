#define SR_TEST_NAME "Test equals operation"
#define SR_TEST_RET_VAL 0x9
#define SR_TEST_FILENAME cmpeq.doff
#define SR_TEST_NUM_STR "00000011"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a == 7) ? 9 : 0;
}

int
test_main()
{

	return test(6) + test(7) + test(8);
}

#endif
