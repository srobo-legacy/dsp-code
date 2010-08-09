#define SR_TEST_NAME "varargs call"
#define SR_TEST_RET_VAL 0xA26
#define SR_TEST_FILENAME varargs.doff
#define SR_TEST_NUM_STR "00000019"

#include "sr_test.h"

#ifndef GENERATE_XML

#include <stdarg.h>
#include <stdint.h>

uint32_t
test(uint32_t first, ...)
{
	va_list l;
	uint32_t val, acc;

	acc = first;
	va_start(l, first);
	while ((val = va_arg(l, uint32_t)) != 0)
		acc += val;

	va_end(l);

	return acc;
}

int
test_main()
{
	uint32_t ret;

	ret = test(8, 7, 33, 85, 2465, 0);
	return ret;
}

#endif
