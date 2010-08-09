#define SR_NODE_GUID_STRING "3CBF0A73-5D16-4B0C-9EF9-3CBAC583886E"
#define SR_NODE_CREATE_FUNC "create"
#define SR_NODE_EXECUTE_FUNC "execute"
#define SR_NODE_DELETE_FUNC "delete"
#define SR_NODE_NAME "bees"
#define SR_NODE_INPUT_STREAMS "1"
#define SR_NODE_OUTPUT_STREAMS "1"

#include <sr_dcd.h>

#include <dspapi.h>

create(int arg_len, char *arg_str, int num_in_streams,
		uint32_t in_stream_handles[], int num_out_streams,
		uint32_t out_stream_handles[], NODE_EnvPtr node)
{

	/* We have precisely no setup */
	return 0;
}

int
execute(NODE_EnvPtr node)
{

	streams = NODE_wait(node, &s->in_handle, 1, 10000, &msgs);
	SYS_printf("sys streams %d, msgs %d\n", streams, msgs);
	if (streams == 0)
		panic();

	return 0;
}

int
delete(NODE_EnvPtr node)
{

	return 0;
}
