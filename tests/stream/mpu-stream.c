#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbapi.h>

bool dsp_open = false;
DSP_HPROCESSOR dsp_handle = NULL;

int
check_dsp_open()
{
	DBAPI status;

	if (dsp_open)
		return 0;

	status = DspManager_Open(0, NULL);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't open dsp manager, %X\n", status);
		return 1;
	}

	status = DSPProcessor_Attach(0, NULL, &dsp_handle);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't attach to dsp 0: %X\n", status);
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

DSP_HNODE
register_and_create(struct DSP_UUID *uuid)
{
	struct DSP_MSG msg;
	DSP_HNODE node;
	DBAPI status, retval;

	retval = 0xFACEBEE5;

	/* Generate a uuid to feed bridge api */
	uuid->ulData1 = 0x7129518D;
	uuid->usData2 = 0xCB99;
	uuid->usData3 = 0x449C;
	uuid->ucData4 = 0x80;
	uuid->ucData5 = 0x20;
	uuid->ucData6[0] = 0x5B;
	uuid->ucData6[1] = 0xBE;
	uuid->ucData6[2] = 0x90;
	uuid->ucData6[3] = 0xCA;
	uuid->ucData6[4] = 0xD8;
	uuid->ucData6[5] = 0xA0;
	DSPManager_UnregisterObject(uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(uuid, DSP_DCDLIBRARYTYPE);

	status = DSPManager_RegisterObject(uuid, DSP_DCDNODETYPE,
						"dsp-stream.doff");
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file, doff missing?\n");
		return NULL;
	}
	status = DSPManager_RegisterObject(uuid, DSP_DCDLIBRARYTYPE,
						"dsp-stream.doff");
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file, doff missing?\n");
		return NULL;
	}

	/* Right - it's registered. Now lets try and run it. */
	status = DSPNode_Allocate(dsp_handle, uuid, NULL, NULL, &node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Failed to allocate node (%X): not a "
				"code problem, something else?\n", status);
		return NULL;
	}

	return node;
}

int
create(DSP_HNODE node)
{
	DBAPI status;

	status = DSPNode_Create(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp node: %X\n", status);
		return 1;
	}

	return 0;
}

int
run(DSP_HNODE node)
{
	DBAPI status;

	status = DSPNode_Run(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't run dsp node: %X\n", status);
		return 1;
	}

	return 0;
}

int
terminate(DSP_HNODE node)
{
	DBAPI status;
	DSP_STATUS retval;

	DSPNode_Delete(node);

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't terminate node, %X\n", status);
		return 1;
	}

	return 0;
}

void
dereg_node(struct DSP_UUID *uuid)
{

	DSPManager_UnregisterObject(uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(uuid, DSP_DCDLIBRARYTYPE);
}

int
main(int argc, char **argv)
{
	struct DSP_UUID uuid;
	struct DSP_STRMATTR attrs;
	DSP_HNODE node;
	DBAPI status;

	if (check_dsp_open()) {
		fprintf(stderr, "Can't open DSP\n");
		return 1;
	}

	/* Register and create the dsp node, but don't create */
	node = register_and_create(&uuid);
	if (node == NULL) {
		fprintf(stderr, "Couldn't allocate dsp node\n");
		return 1;
	}

	/* Create some streams to plug into dsp node */
	attrs.uSegid = DSP_SHMSEG0; /* Allocate in shared mem segment? */
	attrs.uBufsize = 1024; /* Words not byte */
	attrs.uNumBufs = 1;
	attrs.uAlignment = 0;
	attrs.uTimeout = 10000; /* No idea what scale this is */
	attrs.lMode = STRMMODE_PROCCOPY;

	status = DSPNode_Connect(node, 0, (void*)DSP_HGPPNODE, 0, &attrs);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp output stream, %X\n",
				status);
		return 1;
	}

	status = DSPNode_Connect((void*)DSP_HGPPNODE, 0, node, 0, &attrs);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp intpu stream, %X\n",
				status);
		return 1;
	}

	/* Hmkay, now it should be possible to create and execute node */
	if (create(node))
		goto out;

	if (run(node))
		goto out;

	/* XXX - do some stuff with streams */

	out:
	terminate(node);
	dereg_node(&uuid);
	close_dsp();

	return 0;
}
