#define SR_TEST_NAME "Test < operation"
#define SR_TEST_RET_VAL 0x8
#define SR_TEST_FILENAME cmplt.doff
#define SR_TEST_NUM D
#define SR_TEST_NUM_STR "0000000D"

#include "sr_test.h"

#ifndef GENERATE_XML

int
test(int a)
{

	return (a < 4) ? 4 : 0;
}

int
test_main()
{

	return test(2) + test(3) + test(4) + test(5);
}

#endif
