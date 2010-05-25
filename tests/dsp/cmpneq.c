#define SR_TEST_NAME "Test not-equals operation"
#define SR_TEST_RET_VAL 0x12
#define SR_TEST_FILENAME cmpneq.doff
#define SR_TEST_NUM_STR "00000011"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a != 7) ? 9 : 0;
}

int
test_main()
{

	return test(6) + test(7) + test(8);
}

#endif
