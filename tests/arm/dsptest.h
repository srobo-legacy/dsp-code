#include <stdbool.h>
#include <dbapi.h>

int run_test(const char *testname, char *filename, int testnum,
                                                int wanted_retval);

extern bool dsp_open;
extern DSP_HPROCESSOR dsp_handle;

int check_dsp_open();
void close_dsp();
