
struct trap_frame {
        uint32_t ctrl_regs[32];
        uint32_t a_regs[32];
        uint32_t b_regs[32];
        uint32_t orig_sp;
        uint32_t intr;
};

struct dspbridge_fault {
	uint32_t magic1;
#define FAULT_MAGIC1		0xa5a5a5a5
	uint32_t magic2;
#define FAULT_MAGIC2		0x96969696
	uint32_t sz;		/* Size of the whole fault struct */
	uint32_t exc_type;	/* 0 -> intr, 1 -> excp */
	uint32_t fault_ctx;	/* 0 -> task 1 -> swi, 2 -> hwi, 3 -> unknown */
	uint32_t task_handle;	/* Dunno, is printed as hex */
	uint32_t sp;
	uint32_t stack_top;
	uint32_t stack_bottom;
	uint32_t stack_sz;
	uint32_t stack_size;
	uint32_t stack_size_used;
	/* GP Registers */
	uint32_t side_a_regs[32];
	uint32_t side_b_regs[32];
	/* Control Registers */
	uint32_t efr;
	uint32_t ierr;
	uint32_t itsr;
	uint32_t ntsr;
	uint32_t irp;
	uint32_t nrp;
	uint32_t amr;
	uint32_t ilc;
	uint32_t rilc;
	uint32_t ier;
	uint32_t csr;
	/* Stack array - infinitely sized, the contents of this are printed
	 * until the end of this struct, as specified by the sz field, is
	 * reached */
	uint32_t stack[1];
};

void sr_trap(struct trap_frame *frame);
void sr_hacks_isr();

