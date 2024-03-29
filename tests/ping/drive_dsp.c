/* Copyright 2010 Jeremy Morse <jeremy.morse@gmail.com>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY JEREMY MORSE ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL JEREMY MORSE OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbapi.h>

static bool dsp_open = false;
static DSP_HPROCESSOR dsp_handle = NULL;
static DSP_HNODE node;

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
register_and_alloc_node(struct DSP_UUID *uuid, char *filename)
{
	DBAPI status;

	DSPManager_UnregisterObject(uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(uuid, DSP_DCDLIBRARYTYPE);

	status = DSPManager_RegisterObject(uuid, DSP_DCDNODETYPE, filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register dsp code with bridgedriver, "
				"%X\n", (int)status);
		return 1;
	}

	status = DSPManager_RegisterObject(uuid, DSP_DCDLIBRARYTYPE, filename);
	if (DSP_FAILED(status)) {
		fprintf(stderr, "Couldn't register dsp code with bridgedriver, "
				"%X\n", (int)status);
		return 1;
	}

	/* Right - it's registered. Now lets try and run it. */
	status = DSPNode_Allocate(dsp_handle, uuid, NULL, NULL, &node);
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
	int retval;

	status = DSPNode_Delete(node);
	if (DSP_FAILED(status))
		fprintf(stderr, "Error deleting dsp node, %X\n", (int)status);

	status = DSPNode_Terminate(node, &retval);
	if (DSP_FAILED(status))
		fprintf(stderr, "Can't terminate dsp node, %X\n", (int)status);

	return 0;
}

void
dereg_node(struct DSP_UUID *uuid)
{

	DSPManager_UnregisterObject(uuid, DSP_DCDNODETYPE);
	DSPManager_UnregisterObject(uuid, DSP_DCDLIBRARYTYPE);
}

DSP_HNODE
open_dsp_and_node(struct DSP_UUID *uuid, char *filename)
{
	DBAPI status;

	if (check_dsp_open()) {
		fprintf(stderr, "Couldn't open DSP\n");
		return NULL;
	}

	/* Register and allocate the dsp node, but don't create */
	if (register_and_alloc_node(uuid, filename)) {
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
wind_up_dsp(struct DSP_UUID *uuid)
{

	/* We assume there's nothing in flight while we're shutting down */
	terminate(node);
	dereg_node(uuid);
	close_dsp();
	return;
}
