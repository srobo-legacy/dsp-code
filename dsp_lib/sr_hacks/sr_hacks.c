#include <string.h>

#include <sr_hacks.h>
#include "internal.h"

static const uint32_t isr_template[8] =
{ 0x003c20f6, 0x0034a35a, 0x003c02f6, 0x0052502a, 0x001000ea, 0x00000362,
  0x003c02e6, 0x00006000 };

void
install_isr(void (*isr)(), int idx)
{
	uint32_t *istp, *our_isr;
	uint32_t high_addr, low_addr;
	int i;

	istp = getistp();
	our_isr = istp + (idx * 8);	/* Pointer to vector we'll be writing */

	/* Generate an ISR - the hex for this corresponds to the following asm
	 * stw     .D2T2  B0,            *-B15(4)
	 * mvk     .L2    0xD,           B0
	 * stw     .D2T2  B0,            *B15
	 * mvk     .S2    0xFFFFA4A0,    B0
	 * mvkh    .S2    0x20010000,    B0
	 * b       .S2    B0
	 * ldw     .D2T2  *B15,          B0
	 * nop            0x3
	 */
	for (i = 0; i < 8; i++)
		our_isr[i] = isr_template[i];

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
	our_isr[1] &= ~(0x1F << 18);
	our_isr[1] |= idx << 18;

	/* And that's the ISR installed. Now unmask interrupt */
	setier(getier() | (1 << idx));
	return;
}

/* "Public" routines for installing our custom exception and intr handler
 * over the dspbridge version */
void
sr_hacks_install_sr_excp_isr()
{

	install_isr(sr_hacks_isr, 1); /* Intr#1 -> NMI, exception interrupt */
	return;
}

void
sr_hacks_install_sr_gpt8_isr()
{

	install_isr(sr_hacks_isr, 12);
	/* FIXME: also route gpt itr to #12, and unmask that event */
	/* UPDATE: I've tried enabling intr 12 and tying it to trigger on
	 * event 54, the GPTIMER intr, however nothing seems to happen. I've
	 * also tried using the event combining registers and intrs and nothing
	 * seems to happen there either. Uuurgh. */
	return;
}
