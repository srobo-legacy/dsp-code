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

	status = DSPManager_RegisterObject(uuid, DSP_DCDNODETYPE,filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file \"%s\": missing?\n",
								filename);
		return NULL;
	}
	status = DSPManager_RegisterObject(uuid, DSP_DCDLIBRARYTYPE, filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register file \"%s\": missing?\n",
								filename);
		return NULL;
	}

	/* Right - it's registered. Now lets try and run it. */
	status = DSPNode_Allocate(dsp_handle, uuid, NULL, NULL, &node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Failed to allocate node for \"%s\": not a "
				"code problem, something else?\n", filename);
		return NULL;
	}

	status = DSPNode_Create(node);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't create dsp node: %X\n", status);
		return NULL;
	}

	return Node;
}

int
run(DSP_HNODE node)
{

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

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't terminate node, %X\n", status);
		return 1;
	}

	return 0;
}

void
delete_and_dereg_node(DSP_HNODE node, struct DSP_UUID *uuid)
{

	DSPNode_Delete(node);
	DSPManager_UnregisterObject(uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(uuid, DSP_DCDLIBRARYTYPE);
}
