#define SR_TEST_NAME "Test shru operation"
#define SR_TEST_RET_VAL 0x00FF0000
#define SR_TEST_FILENAME shru.doff
#define SR_TEST_NUM_STR "00000009"

#include "sr_test.h"

#ifndef GENERATE_XML

unsigned int
test(unsigned int a)
{

	return a >> 8;
}

unsigned int
test_main()
{

	return test(0xFF000000);
}

#endif
