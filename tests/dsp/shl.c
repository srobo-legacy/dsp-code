#define SR_TEST_NAME "Test shl operation"
#define SR_TEST_RET_VAL 0x755400
#define SR_TEST_FILENAME shl.doff
#define SR_TEST_NUM_STR "00000008"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return a << 8;
}

int
test_main()
{

	return test(0x7554);
}

#endif
