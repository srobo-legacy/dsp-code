#define SR_TEST_NAME "Test xor-immediate operation"
#define SR_TEST_RET_VAL 0x3
#define SR_TEST_FILENAME xori.doff
#define SR_TEST_NUM_STR "00000016"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a ^ 1);
}

int
test2(int a)
{

	return (a ^ -1);
}

int
test_main()
{

	return test(1) + test(2) + test(3) + test(4) + test(5)
		+ test2(1) + test2(2) + test2(3);
}

#endif
