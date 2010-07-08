
struct trap_frame {
        uint32_t ctrl_regs[32];
        uint32_t a_regs[32];
        uint32_t b_regs[32];
        uint32_t orig_sp;
        uint32_t intr;
};

void sr_trap(struct trap_frame *frame);
void srhacks_isr();
