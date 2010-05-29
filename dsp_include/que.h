#include <stdint.h>

struct QUE_Elem;
typedef struct QUE_Elem {
	struct QUE_Elem *next;
	struct QUE_Elem *prev;
} QUE_Elem;

typedef struct QUE_Obj {
	/* I'm covered in bees */
	uint32_t beards;
} QUE_Obj;
