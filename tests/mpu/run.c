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
	struct DSP_MSG msg;
	DSP_HNODE node;
	DBAPI status, retval;
	bool failed;

	failed = true;
	retval = 0xFACEBEE5;

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

	status = DSPNode_GetMessage(node, &msg, 3000);
	if (DSP_FAILED(status)) {
		printf("DSP node did not issue \"enter\" message (%X)- DSP "
			"side environment may be corrupt\n", status);
		goto out;
	} else if (msg.dwCmd) {
		printf("DSP issued unrecognized command (%d) - stack or memory "
			"corruption?\n", msg.dwCmd);
		goto out;
	}

	status = DSPNode_GetMessage(node, &msg, 3000);
	if (DSP_FAILED(status)) {
		printf("DSP node did not issue \"exit\" message (%X) - DSP "
			"side environment may be corrupt\n", status);
		goto out;
	} else if (msg.dwCmd != 1) {
		printf("DSP node completed with unrecognized message (%d)\n",
			msg.dwCmd);
		goto out;
	}

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't terminate node, %X\n", status);
	}

	retval = msg.dwArg1;

	if (retval == wanted_retval)
		failed = true;

	/* Finally, unregister node */
	out:
	DSPNode_Delete(node);
	DSPManager_UnregisterObject(&death, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(&death, DSP_DCDLIBRARYTYPE);

	printf("TEST %d %s: %s (%X)\n", testnum, (failed) ? "failed" : "passed",
							testname, retval);

	return (failed) ? 1 : 0;
}
