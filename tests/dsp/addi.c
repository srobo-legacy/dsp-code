#define SR_TEST_NAME "Test add-immediate operation"
#define SR_TEST_RET_VAL 0x17
#define SR_TEST_FILENAME addi.doff
#define SR_TEST_NUM_STR "00000012"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a + 15);
}

int
test2(int a)
{

	return (a + (-16));
}

int
test_main()
{

	return test(18) + test2(6);
}

#endif
