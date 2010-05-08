#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbapi.h>

#include "dsptest.h"

int
run_test(const char *testname, const char *filename, int testnum,
						int wanted_retval)
{
	struct DSP_UUID death;
	DSP_HNODE node;
	DBAPI status;
	bool failed;

	failed = false;

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

	status = DSPManager_RegisterObject(&death, DSP_DCDNODETYPE, filename);
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
		failed = true;
	} else if (status != wanted_retval) {
		failed = true;
	}

	/* Mkay, that's run the test - to appease the bridgedriver we also need
	 * to kill the node we just created, which could make anything happen.
	 * Or just nothing happen. Anyway, we don't care that much, certainly
	 * not what the return value is */
	DSPNode_Delete(node);

	/* Finally, unregister node */
	out:
	DSPManager_UnregisterObject(&death, DSP_DCDNODETYPE);

	return (failed) ? 1 : 0;
}
