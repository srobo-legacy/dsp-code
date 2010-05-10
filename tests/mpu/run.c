#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dbapi.h>

#include "dsptest.h"

int
run_test(const char *testname, char *filename, int testnum,
						int wanted_retval)
{
	struct DSP_UUID death;
	struct timespec time;
	DSP_HNODE node;
	DBAPI status, retval;
	bool failed;

	failed = true;

	/* Seeing how we're likely to rebuild and munge binaries quite often,
	 * we start testing with unregistering then registering the binary
	 * containing the test. */

	/* Generate a uuid to feed bridge api */
	death.ulData1 = 0xFACEBEE5;
	death.usData2 = 0xFACE;
	death.usData3 = 0xBEE5;
	death.ucData4 = 0xFA;
	death.ucData5 = 0xCE;
	death.ucData6[0] = 0xBE;
	death.ucData6[1] = 0xE5;
	death.ucData6[2] = (testnum >> 24) & 0xFF;
	death.ucData6[3] = (testnum >> 16) & 0xFF;
	death.ucData6[4] = (testnum >> 8) & 0xFF;
	death.ucData6[5] = testnum & 0xFF;

	DSPManager_UnregisterObject(&death, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(&death, DSP_DCDLIBRARYTYPE);

	status = DSPManager_RegisterObject(&death, DSP_DCDNODETYPE,filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file \"%s\": missing?\n",
								filename);
		return -1;
	}
	status = DSPManager_RegisterObject(&death, DSP_DCDLIBRARYTYPE,filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file \"%s\": missing?\n",
								filename);
		return -1;
	}

	/* Right - it's registered. Now lets try and run it. */
	status = DSPNode_Allocate(dsp_handle, &death, NULL, NULL, &node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Failed to allocate node for \"%s\": not a "
				"code problem, something else?\n", filename);
		goto out;
	}

	status = DSPNode_Create(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp node: %X\n", status);
		goto out;
	}

	/* That will have run the sr default create function - now run execute
	 * phase and get its return value */
	status = DSPNode_Run(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't run dsp node: %X\n", status);
		goto out;
	}

	/* Wait a bit */
	time.tv_sec = 0;
	time.tv_nsec = 100000000; /* 100ms */
	nanosleep(&time, NULL);

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't terminate node, %X\n", status);
	} else if (retval == wanted_retval) {
		failed = false;
	}

	/* Finally, unregister node */
	out:
	DSPNode_Delete(node);
	DSPManager_UnregisterObject(&death, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(&death, DSP_DCDLIBRARYTYPE);

	printf("TEST %d %s: %s\n", testnum, (failed) ? "failed" : "passed",
								testname);

	return (failed) ? 1 : 0;
}
