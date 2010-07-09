/* This routine is called by the sr ISR code, which dumps all register state to
 * the stack and passes in a pointer. Here we pump it out into the dsp trace
 * buffer so that it can be printed back to the user to be vaguely informative.
 * If we inspect the stored information and discover that we've been invoked
 * through the exception handler, then we first post an exception message to the
 * MPU and pause a little while */

#include <stdint.h>
#include <string.h>
#include <srhacks.h>
#include "internal.h"

void SYS_printf(const char *format, ...);

void __attribute__((noreturn))
sr_trap(struct trap_frame *frame)
{
	struct dspbridge_fault *fault;
	int i;

	/* I've never had a legitimate reason to use this, ever, so I'll be
	 * damned if I miss this one out -
	 *
	 *                                             ___________
	 *                                           /             \
	 *                                           | It’s a trap! |
	 *                      _,,,--~~~~~~~--,_     \  ___________/
	 *                    ,-‘                ‘-,   \/
	 *                 ,-‘                   o : ‘-,
	 *               ,-‘                        :O ‘-,
	 *             ,-‘                              ’;
	 *           ,-‘ //               -,              \
	 *          /,‘,’                  ‘-,            |
	 *         /,’,‘               _,-~~,_’-,         |
	 *       _/,’ /           _,-‘/ ,-‘;’’’~-,       /
	 *    ,-‘/     ,-‘    ,--‘’| / ,-‘-‘__’’’ \    ,-
	 *     \ /   _,-‘--,,  \  \ |/ /,-‘x###  \ \    /
	 *     \//-- \ #\   \  \  \| |   (O##º  / /  ‘’
	 *     /’____ \ #\   \  \  \ \ \  ‘-,_,-‘--,-'
	 *      )    ’’-,--,, ¯\ \     -- _ -¯ -- , - -
	 *     )      ,    ’’~~~’ \        ’’¯   ,-‘  /\
	 *     \ //|\\|| //      ’-,           ,-  ,-‘ \\
	 *      \\|\\\|/// _--   | )        ,-‘’,-‘     \\
	 *     /¯|\|\|/\   --- \/  |      ,-’ ,-‘        ‘’-,,_
	 *    |   / -(,    ’’’~,,,’’    ,-‘ ,-‘           ,-‘’’\\
	 *   -   |  ‘’)   ¯’’~      ,--’ ,-’            ,-  ¯   ’-,_
	 *  /    ’-,  |    _,,-‘’’’¯ ,--‘              /           ’’-,
	 * /     -   ¯’’’¯    _,,-~’’                 |
	 *        ¯’’~~~~~’’’                         |
	 *
	 * Credit for original ascii, supernaturalhigh
	 */


	/* So, we can arrive here in the case of two interrupts: GPT8 and if
	 * an exception occurs. We can work this out from what ID they pushed
	 * on the stack in the short service routine */
	if (frame->intr == 1) {
		/* It was an exception - notify mpu and wait for it to read the
		 * trace buffer */
		write_reg_32(0x11808044, 0x102);
		for (i = 0; i < 100000; i++)
			;
	}

	/* Load pointer to trace buffer - this is liable to change between
	 * releases of TIs dynbase firmware, but there's precious little that
	 * can be done about that. Presumably a future release won't require
	 * this hack :| */
	fault = (struct dspbridge_fault*)0x1100b000;

	/* Write the contents of the trap frame to the trace buffer */
	memcpy(&fault->side_a_regs, &frame->a_regs, 32 * 4);
	memcpy(&fault->side_b_regs, &frame->b_regs, 32 * 4);
	/* Ctrl registers are identified by their register address */
	fault->efr = frame->ctrl_regs[29];
	fault->ierr = frame->ctrl_regs[31];
	fault->itsr = frame->ctrl_regs[27];
	fault->ntsr = frame->ctrl_regs[28];
	fault->irp = frame->ctrl_regs[6];
	fault->nrp = frame->ctrl_regs[7];
	fault->amr = frame->ctrl_regs[0];
	fault->ilc = frame->ctrl_regs[13];
	fault->rilc = frame->ctrl_regs[14];
	fault->ier = frame->ctrl_regs[4];
	fault->csr = frame->ctrl_regs[1];

	/* Now write out a few other bits of information bridgedriver wants */
	fault->exc_type = (frame->intr == 1) ? 1 : 0;
	fault->fault_ctx = 0;	/* We don't know what this is anyway */
	fault->task_handle = 0;	/* Don't know this either */
	fault->sp = frame->orig_sp + 4;

	/* Presumably these bits of info could be pulled out of the dspbios
	 * task state, but I've no wish to interface with that */
	fault->stack_top = 0;
	fault->stack_bottom = 0;
	fault->stack_sz = 0;
	fault->stack_size_used = 0;

	/* Size of fault data depends on how much stack we write out - for now
	 * we won't bother writing any stack */
	fault->sz = sizeof(*fault) - 4;

	/* Write magic numbers indicating this data is written out; we do this
	 * last to ensure that we don't get in a race with the mpu */
	fault->magic1 = FAULT_MAGIC1;
	fault->magic2 = FAULT_MAGIC2;

	/* Enter an infinite loop - there's no way to recover from faults,
	 * it's the users problem not ours. Restart is achieved by plugging
	 * the driver. */
	while (1)
		;
}
