#ifdef GENERATE_XML
	<test>
		<name>SR_TEST_NAME</name>
		<retval>SR_TEST_RET_VAL</retval>
		<filename>SR_TEST_FILENAME</filename>
		<nodenum>SR_TEST_NUM</nodenum>
	</test>
#else

#define SR_NODE_GUID_STRING "FACEBEE5_FACE_BEE5_FACE_BEE5" SR_TEST_NUM_STR
#define SR_NODE_CREATE_FUNC "main"
#define SR_NODE_EXECUTE_FUNC "main"
#define SR_NODE_DELETE_FUNC "main"
#define SR_NODE_NAME SR_TEST_NAME

#include "sr_dcd.h"

#endif
