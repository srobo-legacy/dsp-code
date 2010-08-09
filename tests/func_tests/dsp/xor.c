#define SR_TEST_NAME "Test xor operation"
#define SR_TEST_RET_VAL 0xAAAA
#define SR_TEST_FILENAME xor.doff
#define SR_TEST_NUM_STR "0000000C"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a ^ 0x5555;
}

int
test_main()
{

	return test(0xFFFF);
}

#endif
