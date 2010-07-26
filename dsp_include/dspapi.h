/* This file contains a set of definitions and prototypes used for accessing
 * the DSP-side API of TIs dspbridge. This isn't intended to be comprehensive
 * or even particularly detailed, instead this contains enough definitions to
 * allow you to interface with the stream framework and memory facilities
 * provided on the DSP.
 *
 * Documentation of what the routines here do is not provided - it exists
 * elsewhere on the internet. Almost-full-but-not-quite headers exist in the
 * userspace-dspbridge repo that TI manage, as well as some sparse
 * documentation, but I don't believe they're redistributable.
 *
 * Ultimately if you want to use the more complex data structures and interfaces
 * that exist on the DSP side, you need to fetch and use DSPBios itself. */

/* Include host standard integer definitons... on the hope that they're sane */
#include <stdbool.h>
#include <stdint.h>

#define NULL ((void *)0)

/* Definitions */

/* The following are a series of types that the normal DSPBridge headers define,
 * and that might occur in existing DSP code. On the whole, C99 fixed width
 * types are approximately one bajillion times nicer than using these */

typedef void Void;
typedef char SmInt;
typedef short MdInt;
typedef int LgInt;
typedef unsigned char SmUns;
typedef unsigned short MdUns;
typedef unsigned int LgUns;
typedef int Int;
typedef long int Long;
typedef short int Short;
typedef char Char;
typedef char *String;
typedef unsigned int Uns;
typedef void *Ptr;
typedef int Arg;
typedef int(*Fxn)();
typedef float Float;

typedef unsigned int RMS_WORD;
typedef char RMS_CHAR;
typedef unsigned int RMS_STATUS;
typedef unsigned int (*LgFxn)();

#define RMS_SBASE	0x8000
#define RMS_EBASE	0x80008000

#define RMS_EOK		RMS_SBASE

#define RMS_EFAIL	(RMS_EBASE + 0x8)
#define RMS_EFREE	(RMS_EBASE + 0x2A)
#define RMS_EIOFREE	(RMS_EBASE + 0x2B)
#define RMS_EMULINST	(RMS_EBASE + 0x2C)
#define RMS_ENOTFOUND	(RMS_EBASE + 0x2D)
#define RMS_ENOTIMPL	(RMS_EBASE + 0x10)
#define RMS_EOUTOFIO	(RMS_EBASE + 0x2E)
#define RMS_EOUTOFMEMORY (RMS_EBASE + 0xC)
#define RMS_ERESOURCE	(RMS_EBASE + 0x28)
#define RMS_ESTREAM	(RMS_EBASE + 0x15)
#define RMS_ETASK	(RMS_EBASE + 0x16)
#define RMS_ENODEHEAP	(RMS_EBASE + 0x17)

typedef struct {
	void *message;
	void *moreEnv;
} RMS_MsgEnv;

typedef RMS_MsgEnv * NODE_EnvPtr;

typedef struct {
	RMS_WORD	cmd;
	RMS_WORD	arg1;
	RMS_WORD	arg2;
} RMS_DSPMSG;

typedef struct {
	RMS_WORD	bufsize;
	RMS_WORD	nbufs;
	RMS_WORD	segid;
	RMS_WORD	align;
	RMS_WORD	timeout;
	RMS_CHAR	name[1];
} RMS_StrmDef;

typedef void *STRM_Handle;

typedef struct STRM_Attrs {
	int	nbufs;
	int	segid;
	int	align;
	int	timeout;
} STRM_Attrs;

typedef enum {
	STRM_INPUT = 0,
	STRM_OUTPUT = 1
} STRM_DIRECTION;

extern STRM_Attrs STRM_ATTRS; /* Default settings */

typedef struct BCACHE_Size {
	unsigned int l1psize;
	unsigned int l1dsize;
	unsigned int l2size;
} BCACHE_Size;

#define RMS_EXIT		0x80000000
#define RMS_EXITACK		0x40000000
#define RMS_USER		0
#define RMS_MAXUSERCODES	0xFFF

#define NODE_FOREVER		((unsigned int)-1)
#define NODE_TOGPP		0

#define DBC_ensure(x)		/* Clobber assertions */

/* Function prototypes */

/* Print a message to dsp trace buffer */
void SYS_printf(const char *fmt, ...);

/* Memory routines. If in doubt, pass -1 as the seg parameter */
void *MEM_alloc(int seg, int size, int align);
void *MEM_valloc(int seg, int size, int align, char val);
bool MEM_free(int seg, void *addr, int size);

#define MEM_calloc(seg, sz, align) MEM_valloc(seg, sz, align, 0)
#define MEM_ILLEGAL NULL

/* Node management */
unsigned int NODE_getPri(NODE_EnvPtr node);
void *NODE_getHeapAddr(NODE_EnvPtr node);
int NODE_getHeapSeg(NODE_EnvPtr node);
void *NODE_allocMsgBuf(NODE_EnvPtr node, unsigned int size, unsigned int align);
bool NODE_freeMsgBuf(NODE_EnvPtr node, void *addr, unsigned int size);
bool NODE_getMsg(NODE_EnvPtr node, RMS_DSPMSG *msg, unsigned int timeout);
bool NODE_putMsg(NODE_EnvPtr src, NODE_EnvPtr dst, RMS_DSPMSG *msg,
						unsigned int timeout);
unsigned int NODE_wait(NODE_EnvPtr node, STRM_Handle *streams,
					unsigned int num_streams,
					unsigned int timeout,
					unsigned int *num_recvd_msgs);

/* Stream interface */
void *STRM_allocateBuffer(STRM_Handle stream, unsigned int size);
STRM_Handle STRM_create(char *name, STRM_DIRECTION mode, unsigned int size,
					STRM_Attrs *attrs);
int STRM_delete(STRM_Handle stream);
bool STRM_freeBuffer(STRM_Handle stream, void *addr, unsigned int size);
int STRM_idle(STRM_Handle stream, bool flush);
int STRM_issue(STRM_Handle stream, void *buf, unsigned int data_sz,
					unsigned int buffer_size,
					int arg);
unsigned int STRM_reclaim(STRM_Handle stream, void **buffer,
					unsigned int *buffer_sz,
					int *arg);

/* Cache management, names are self explanatory */
void BCACHE_wb(void *block, unsigned int size, bool wait);
void BCACHE_inv(void *block, unsigned int size, bool wait);
void BCACHE_wbInv(void *block, unsigned int size, bool wait);
void BCACHE_wbAll();
void BCACHE_wbInvAll();
void BCACHE_wait(void);
void BCACHE_getSize(BCACHE_Size *size);
void BCACHE_setSize(BCACHE_Size *size);
