/* Values you need to define when including this header, as strings:
 * SR_NODE_GUID_STRING: nodes guid in the form:
 * 	 "00000000_0000_0000_0000_000000000000"
 * SR_NODE_CREATE_FUNC: Function name to be called for node creation
 * SR_NODE_EXECUTE_FUNC: Function name to be called for node execution
 * SR_NODE_DELETE_FUNC: Function name to be called for note deletion */

#ifndef _TESTS_DSP_SR_DCD_H_
#define _TESTS_DSP_SR_DCD_H_

#ifndef SR_NODE_INPUT_STREAMS
#define SR_NODE_INPUT_STREAMS "0"
#endif
#ifndef SR_NODE_OUTPUT_STREAMS
#define SR_NODE_OUTPUT_STREAMS "0"
#endif

#define SR_STATIC_DATA_SZ "1024"
#define SR_GLOBAL_DATA_SZ "512"
#define SR_PROG_MEM_SZ "128"

/* Generate dcd_register section - this contains one or more comma seperated (?)
 * guids to be found and registered by dynreg */
static const char __attribute__((section(".dcd_register"))) sr_dcd_register[] =
        SR_NODE_GUID_STRING ":0";

/* deathstring: this comma seperated list of values is fed to the bridgedriver,
 * which extracts various pieces of information from it, see below */
static const char __attribute__((section("." SR_NODE_GUID_STRING))) sr_death[] =
        "1024," SR_NODE_GUID_STRING ",nodename,"
	"1,0," SR_STATIC_DATA_SZ "," SR_GLOBAL_DATA_SZ "," SR_PROG_MEM_SZ
	",3072,5,3,1000,100,10,1,4096,128,0,2,"
	SR_NODE_INPUT_STREAMS ","
	SR_NODE_OUTPUT_STREAMS ","
	"7d0H," /* timeout */
	SR_NODE_CREATE_FUNC ","
	SR_NODE_EXECUTE_FUNC ","
	SR_NODE_DELETE_FUNC
	",0,32768,sr_test_node"	/* node name must not contain a space, hangs */
	",1,ff3f3f3fH,ff3f3f3fH,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,stackSegName,";

/* llvm-gcc helpfully optimises the above out if there isn't a reference
 * in .text or data - so ensure a reference. */
const void *sr_dcd_ptr = sr_dcd_register;
const void *sr_death_ptr = sr_death;

/* Data is supposed to contain the following, names from a TI script:
 * 	1024,`guid`,`nodeTypeVal`,`bCacheVal`,
 *      `a.dspr_cbStruct`,`a.dspr_uStaticDataSize`,`a.dspr_uGlobalDataSize`,
 *      `a.dspr_ProgMemSize`,`a.dspr_uWCExecutionTime`,`a.dspr_uWCPeriod`,
 *      `a.dspr_uWCDeadline`,`a.dspr_uAvgExecutionTime`,`a.dspr_MinPeriod`,
 *      `a.priority`,`a.stackSize`,`a.sysStackSize`,`a.stackSeg`,
 *      `a.messageDepth`,`a.numInputStreams``a.numOutputStreams`,
 *      `dspbridge.toHex(a.timeout)`,`a.createFxn`,`a.executeFxn`,`a.deleteFxn`,
 *      `a.uMsgSegid`,`notifyType`,`a.algStructName`,`dynLoadVal`,
 *      `dspbridge.toHex(a.dataMask)`,`dspbridge.toHex(a.codeMask)`,
 *      `a.heapSizeProfile{0-15}`,`a.stackSegName`,
 *
 *      Some notes - fxn names are just symbols to be called for various opers.
 *      the data and code masks are a set of bits in three octets, the lower 6
 *      of which specify the segment create/execute/delete has to live in; and
 *      the seventh bit indicates if that's mandatory (1) or optional.
 *      Need to set timeout accurately (7d0H in example) else SendMessage
 *      and or WMD_CHNL_GetIOC returns an error complaining about bad timeouts
 *      (and the error path of which panics.
 *      Node Name musn't have a space in it, or bridgedriver hangs in strtok.
 *      For everything else, we have the names of the values, but for what they
 *      do and which ones bridgedriver honours, your guess is as good as mine
 *      stackSegName must be important, if it's changed all node communication
 *      times out. No idea why. */

#else /*_TESTS_DSP_SR_DCD_H_*/
#error Dont include sr_dcd.h more than once, dspbridge will hate you
#endif
