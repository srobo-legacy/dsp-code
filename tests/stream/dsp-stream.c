#define SR_NODE_GUID_STRING "7129518D_CB99_449C_8020_5BBE90CAD8A0"
#define SR_NODE_CREATE_FUNC "create"
#define SR_NODE_EXECUTE_FUNC "execute"
#define SR_NODE_DELETE_FUNC "delete"
#define SR_NODE_INPUT_STREAMS "1"
#define SR_NODE_OUTPUT_STREAMS "1"

#include <sr_dcd.h>

#include <dspapi.h>

struct state {
	STRM_Handle in_handle;
	unsigned int in_size;
	STRM_Handle out_handle;
	unsigned int out_size;
	uint8_t *in_buf;
	uint8_t *out_buf;
};

void
panic()
{
	int *beards;

	beards = (void*) 0xFACEBEE5;
	*beards = 0;
	return;
}

int
create(int arg_len, char *arg_str, int num_in_streams,
		uint32_t in_stream_handles[], int num_out_streams,
		uint32_t out_stream_handles[], NODE_EnvPtr node)
{
	STRM_Attrs attrs;
	struct state *s;
	RMS_StrmDef *strm_def;

	s = MEM_valloc(0, sizeof(struct state), 0, 0);
	node->moreEnv = s;
	if (s == NULL)
		return RMS_EOUTOFMEMORY;

	strm_def = (RMS_StrmDef *)in_stream_handles[0];
	attrs.nbufs = strm_def->nbufs;
	attrs.segid = strm_def->segid;
	attrs.timeout = strm_def->timeout;
	attrs.align = strm_def->align;

	s->in_handle = STRM_create(strm_def->name, STRM_INPUT,
					strm_def->bufsize, &attrs);
	s->in_size = strm_def->bufsize;

	strm_def = (RMS_StrmDef *)out_stream_handles[0];
	attrs.nbufs = strm_def->nbufs;
	attrs.segid = strm_def->segid;
	attrs.timeout = strm_def->timeout;
	attrs.align = strm_def->align;

	s->out_handle = STRM_create(strm_def->name, STRM_OUTPUT,
					strm_def->bufsize, &attrs);
	s->out_size = strm_def->bufsize;

	s->in_buf = (void *)STRM_allocateBuffer(s->in_handle, s->in_size);
	s->out_buf = (void *)STRM_allocateBuffer(s->out_handle,s->out_size);

	if (s->in_buf == NULL || s->out_buf == NULL)
		return RMS_EOUTOFMEMORY;

	return 0;
}

int
execute(NODE_EnvPtr node)
{
	struct state *s;
	uint8_t *in_buf;
	uint32_t context;
	Uns streams, msgs; /* Uns? Sounds like a war crime to me */
	int i;

	s = node->moreEnv;
	/* "Prime" input stream with buffer (as said by strm example) */
	STRM_issue(s->in_handle, s->in_buf, s->in_size, s->in_size, 0);

	streams = NODE_wait(node, &s->in_handle, 1, 10000, &msgs);
	SYS_printf("sys streams %d, msgs %d\n", streams, msgs);
	if (streams == 0)
		panic();

	STRM_reclaim(s->in_handle, &in_buf, NULL, &context);

	if (in_buf == NULL)
		panic();

	for (i = 0; i < 1024; i++) {
		s->out_buf[i] = 1;
	}

	STRM_issue(s->out_handle, s->out_buf, 1024, 1024, 0);

	return 0;
}

int
delete(NODE_EnvPtr node)
{
	struct state *s;

	s = node->moreEnv;
	STRM_freeBuffer(s->in_handle, s->in_buf, s->in_size);
	STRM_freeBuffer(s->out_handle, s->out_buf, s->out_size);
	STRM_delete(s->in_handle);
	STRM_delete(s->out_handle);
	MEM_free(0, s, sizeof(*s));

	return 0;
}
