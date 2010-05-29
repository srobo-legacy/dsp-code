#define SR_NODE_GUID_STRING "FACEBEE5_FACE_BEE5_FACE_BEE5AAAAAAAA"
#define SR_NODE_CREATE_FUNC "create"
#define SR_NODE_EXECUTE_FUNC "execute"
#define SR_NODE_DELETE_FUNC "delete"
#define SR_NODE_NAME "bees"
#define SR_INPUT_STREAMS "1"
#define SR_OUTPUT_STREAMS "1"

#include <std.h>
#include <stdlib.h>
#include <rmsdefs.h>
#include <rms_sh.h>
#include <node.h>

#include <sr_dcd.h>

void *MEM_alloc(int, int, int);

struct state {
	STRM_Handle in_handle;
	unsigned int in_size;
	STRM_Handle out_handle;
	unsigned int out_size;
	void *in_buf;
	void *out_buf;
};

int
create(int arg_len, char *arg_str, itn num_in_streams,
		uint32_t in_stream_handles[], int num_out_streams,
		uint32_t out_stream_handles[], NODE_EnvPtr node)
{
	STRM_Attrs attrs = STRM_ATTRS;
	struct state *s;
	RMS_StrmDef *strm_def;

	s = MEM_alloc(0, sizeof(struct state), 0);
	node->moreEnv = s;
	if (s == NULL)
		return RMS_EOUTOFMEMORY;

	strm_def = (RMS_StrmDef *)in_def[0]
	attrs.nbufs = strm_def->nbufs;
	attrs.segid = strm_def->segid;
	attrs.timeout = strm_def->timeout;
	attrs.align = strm_def->align;

	state->in_handle = STRM_create(strm_def->name, STRM_INPUT,
					strm_def->bufsize, &attrs);
	state->in_size = strm_def->bufsize;

	strm_def = (RMS_StrmDef *)out_def[0]
	attrs.nbufs = strm_def->nbufs;
	attrs.segid = strm_def->segid;
	attrs.timeout = strm_def->timeout;
	attrs.align = strm_def->align;

	state->out_handle = STRM_create(strm_def->name, STRM_OUTPUT,
					strm_dev->bufsize, *attrs);
	state->out_size = strm_def->bufsize;

	s->in_buf = (void *)STRM_allocateBuffer(s->in_handle, s->in_size);
	s->out_buf = (void *)STRM_allocateBuffer(s->out_handle,s->out_size);

	if (state->in_buf == NULL || state->out_buf == NULL)
		return RMS_EOUTOFMEMORY;

	_SYS_printf("Won create phase\n");

	return 0;
}

int
execute(NODE_EnvPtr node)
{

	/* do some stuff */
}

int
delete(NODE_EnvPtr node)
{
	struct state *s;

	s = node->moreEnv;
	MEM_free(0, s, sizeof(*s));

	return 0;
}
