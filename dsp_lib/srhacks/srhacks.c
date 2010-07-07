#include <string.h>

#include <srhacks.h>

void
install_isr(void (*isr)(), int idx)
{
	uint32_t *istp, *mbox_isr, *our_isr;
	uint32_t high_addr, low_addr;

	istp = getistp();
	mbox_isr = istp + 0x1A0;	/* Pointer to mailbox intr vector */
	our_isr = istp + (idx * 0x20);	/* Pointer to vector we'll be writing */

	/* Now then - we could either formulate our own 8-insn handler to bounce
	 * control to an actual ISR, or we could just use the one dspbridge has
	 * already installed and adjust the address it jmps to. */
	memcpy(our_isr, mbox_isr, 0x20);

	/* Split into the two parts of the load */
	high_addr = ((uint32_t)isr) >> 16;
	low_addr = ((uint32_t)isr) & 0xFFFF;

	/* Shift to the field position of mvkl/mvkh */
	high_addr <<= 7;
	low_addr <<= 7;

	/* Remove address from current fields */
	our_isr[3] &= ~(0xFFFF << 7);
	our_isr[4] &= ~(0xFFFF << 7);

	/* And OR ours in */
	our_isr[3] |= low_addr;
	our_isr[4] |= high_addr;

	/* Finally, there's a value pushed onto the stack indicating which
	 * interrupt caused the ISR to be invoked - update this to which intr
	 * we're coming from. */
	our_isr[1] &= ~(0xFFFF << 7);
	our_isr[1] |= idx << 7;

	/* And that's the ISR installed. Now unmask interrupt */
	setier(getier() | (1 << idx));
	return;
}

/* "Public" routines for installing our custom exception and intr handler
 * over the dspbridge version */
void
srhacks_install_sr_excp_isr()
{

	install_isr(srhacks_isr, 1); /* Intr#1 -> NMI, exception interrupt */
	return;
}

void
srhacks_install_sr_gpt8_isr()
{

	install_isr(srhacks_isr, 12);
	/* FIXME: also route gpt itr to #12, and unmask that event */
	return;
}
