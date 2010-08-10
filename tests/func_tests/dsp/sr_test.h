#ifdef GENERATE_XML
	<test>
		<name>SR_TEST_NAME</name>
		<retval>SR_TEST_RET_VAL</retval>
		<filename>SR_TEST_FILENAME</filename>
		<nodenum>SR_TEST_NUM_STR</nodenum>
	</test>
#else

int sr_node_create();
int sr_node_delete();

#define SR_NODE_GUID_STRING "FACEBEE5_FACE_BEE5_FACE_BEE5" SR_TEST_NUM_STR
#define SR_NODE_CREATE_FUNC "main"
#define SR_NODE_EXECUTE_FUNC "run"
#define SR_NODE_DELETE_FUNC "main"

#include <sr_dcd.h>

#endif
