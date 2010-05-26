#define SR_TEST_NAME "Test sub-immediate operation"
#define SR_TEST_RET_VAL 0x13
#define SR_TEST_FILENAME subi.doff
#define SR_TEST_NUM_STR "00000013"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a - 7);
}

int
test2(int a)
{

	return (a - (-12));
}

int
test_main()
{

	return test(11) + test2(3);
}

#endif
