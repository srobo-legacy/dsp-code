#define SR_TEST_NAME "var-arg printf call"
#define SR_TEST_RET_VAL 0x0
#define SR_TEST_FILENAME varprintf.doff
#define SR_TEST_NUM_STR "00000018"

#include "sr_test.h"

#ifndef GENERATE_XML

extern void SYS_printf(const char *format, ...);

int
test_main()
{

	SYS_printf("%s %s\n", "Hello" "world");
	return 0;
}

#endif
