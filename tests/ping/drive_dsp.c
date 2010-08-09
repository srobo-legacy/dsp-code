#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbapi.h>

static bool dsp_open = false;
static DSP_HPROCESSOR dsp_handle = NULL;
static DSP_HNODE node;

static struct DSP_UUID uuid = {0x3CBF0A73, 0x5D16, 0x4B0C, 0x9E, 0xF9,
				{0x3C, 0xBA, 0xC5, 0x83, 0x88, 0x6E}};

int
check_dsp_open()
{
	DBAPI status;

	if (dsp_open)
		return 0;

	status = DspManager_Open(0, NULL);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't open dsp manager, %X\n", (int)status);
		return 1;
	}

	status = DSPProcessor_Attach(0, NULL, &dsp_handle);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't attach to dsp 0: %X\n", (int)status);
		DspManager_Close(0, NULL);
		return 1;
	}

	dsp_open = true;
	return 0;
}

void
close_dsp()
{

	dsp_open = false;
	DSPProcessor_Detach(&dsp_handle);
	DspManager_Close(0, NULL);
	return;
}

int
register_and_alloc_node()
{
	DBAPI status;

	DSPManager_UnregisterObject(&uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(&uuid, DSP_DCDLIBRARYTYPE);

	status = DSPManager_RegisterObject(&uuid, DSP_DCDNODETYPE, "ping.doff");
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register dsp code with bridgedriver, "
				"%X\n", (int)status);
		return 1;
	}

	status = DSPManager_RegisterObject(&uuid,DSP_DCDLIBRARYTYPE,
							"ping.doff");
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register dsp code with bridgedriver, "
				"%X\n", (int)status);
		return 1;
	}

	/* Right - it's registered. Now lets try and run it. */
	status = DSPNode_Allocate(dsp_handle, &uuid, NULL, NULL, &node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Failed to allocate dsp node (%X) from "
				"bridgedriver\n", (int)status);
		return 1;
	}

	return 0;
}

int
terminate(DSP_HNODE node)
{
	DBAPI status;
	DSP_STATUS retval;

	status = DSPNode_Delete(node);
	if (DSP_FAILED(status))
		fprintf(stderr, "Error deleting dsp node, %X\n", (int)status);

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status))
		fprintf(stderr, "Can't terminate dsp node, %X\n", (int)status);

	return 0;
}

void
dereg_node()
{

	DSPManager_UnregisterObject(&uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(&uuid, DSP_DCDLIBRARYTYPE);
}

DSP_HNODE
open_dsp_and_node()
{
	DBAPI status;

	if (check_dsp_open()) {
		fprintf(stderr, "Couldn't open DSP\n");
		return NULL;
	}

	/* Register and allocate the dsp node, but don't create */
	if (register_and_alloc_node()) {
		fprintf(stderr, "Couldn't allocate dsp node\n");
		return NULL;
	}

	/* Hmkay, now it should be possible to create and execute node */
	status = DSPNode_Create(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp node: %X\n", (int)status);
		goto fail;
	}

	status = DSPNode_Run(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't run dsp node: %X\n", (int)status);
		goto fail;
	}

	return node;

	fail:
	DSPNode_Delete(node);

	return NULL;
}

void
wind_up_dsp()
{

	/* We assume there's nothing in flight while we're shutting down */
	terminate(node);
	dereg_node(&uuid);
	close_dsp();
	return;
}
