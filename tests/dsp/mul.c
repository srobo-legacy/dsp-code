#define SR_TEST_NAME "Test mul operatin"
#define SR_TEST_RET_VAL 0xFF00
#define SR_TEST_FILENAME mul.doff
#define SR_TEST_NUM 6
#define SR_TEST_NUM_STR "00000006"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a, int b)
{

	return a * b;
}

int
test_main()
{

	return test(0xFF, 0x100);
}

#endif
