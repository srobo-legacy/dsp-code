#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbapi.h>

DSP_HNODE open_dsp_and_node(struct DSP_UUID *uuid, char *filename);
void wind_up_dsp(struct DSP_UUID *uuid);

struct DSP_UUID ping_uuid = {0x3CBF0A73, 0x5D16, 0x4B0C, 0x9E, 0xF9,
				{0x3C, 0xBA, 0xC5, 0x83, 0x88, 0x6E}};

int
main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	struct DSP_MSG msg;
	DSP_HNODE node;
	DBAPI status;
	int i, num;

	num = 50;
	node = open_dsp_and_node(&ping_uuid, "ping.doff");
	if (node == NULL) {
		fprintf(stderr, "Couldn't open dsp or create node\n");
		return 1;
	}

	/* Main loop - send and receive pings */
	for (i = 0; i < num; i++) {
		msg.dwCmd = i;
		msg.dwArg1 = 0x01234567;
		msg.dwArg2 = 0x89ABCDEF;

		/* Give DSP message - wait for up to 1 second for it to confirm
		 * its receipt */
		status = DSPNode_PutMessage(node, &msg, 1000);
		if (DSP_FAILED(status)) {
			fprintf(stderr, "Couldn't send message %d, 0x%X\n",
						i, (unsigned int) status);
			continue;
		}

		/* Wipe message */
		memset(&msg, 0, sizeof(msg));

		/* Wait for the reply, up to 1 second again */
		status = DSPNode_GetMessage(node, &msg, 1000);
		if (DSP_FAILED(status)) {
			fprintf(stderr, "Couldn't receive back msg %d, 0x%X\n",
						i, (unsigned int) status);
			continue;
		}

		if (msg.dwCmd != i || msg.dwArg1 != 0x89ABCDEF ||
				msg.dwArg2 != 0x01234567) {
			fprintf(stderr, "Received reply to msg %d, however  "
				"response is incorrect (%X %X %X)\n",
				msg.dwCmd, msg.dwArg1, msg.dwArg2);
		} else {
			printf("Successfully received ping %d\n", i);
		}
	}

	wind_up_dsp(&ping_uuid);
	return 0;
}

