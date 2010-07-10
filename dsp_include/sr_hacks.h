#ifndef _DSP_CODE_DSP_INCLUDES_SR_HACKS_H_
#define _DSP_CODE_DSP_INCLUDES_SR_HACKS_H_

#include <stdint.h>

void sr_hacks_install_sr_excp_isr();
void sr_hacks_install_sr_gpt8_isr();

/* Help user to beat registers directly from their code */

/* Write a 32 bit word to an address */
void write_reg_32(uint32_t address, uint32_t value);

/* Read a 32 bit word from an address */
uint32_t read_reg_32(uint32_t address);

#endif
