#define SR_TEST_NAME "Check argument reaches function"
#define SR_TEST_RET_VAL 0x2A
#define SR_TEST_FILENAME argcall.doff
#define SR_TEST_NUM_STR "00000003"

#include "sr_test.h"

#ifndef GENERATE_XML

int
bees(int i)
{

	return i;
}

int
test_main()
{

	return bees(0x2A);
}

#endif
