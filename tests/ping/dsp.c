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
	RMS_DSPMSG msg, msg2;
	int msgs;

	while (1) {
		NODE_wait(node, NULL, 0, 10000, &msgs);
		if (msgs == 0) {
			SYS_printf("No messages after 10s?\n");
			continue;
		}

		NODE_getMsg(node, &msg, NODE_FOREVER);
		if (msg.cmd == RMS_EXIT)
			break;

		/* Copy message into second struct, flipping the args */
		msg2.cmd = msg.cmd;
		msg2.arg1 = msg.arg2;
		msg2.arg2 = msg.arg1;

		/* Send it back */
		NODE_putMsg(node, NODE_TOGPP, &msg2, NODE_FOREVER);

		/* And continue on our merry dance */
	}

	return 0;
}

int
delete(NODE_EnvPtr node)
{

	return 0;
}
