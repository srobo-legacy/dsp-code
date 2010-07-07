#ifndef _DSP_CODE_DSP_LIBS_SRHACKS_SRHACKS_H_
#define _DSP_CODE_DSP_LIBS_SRHACKS_SRHACKS_H_

#include <stdint.h>

void srhack_isr();
void srhack_install_sr_excp_isr();
void srhack_install_sr_gpt8_isr();

/* Some utility functions to allow direct hardware access from C */

/* Fetch pointer to interrupt service table */
void *getistp();

/* Fetch interrupt enable register */
uint32_t getier();

/* Set interrupt enable register */
void setier(uint32_t ie_mask);

/* Write a 32 bit word to an address */
void write_reg_32(uint32_t address, uint32_t value);

/* Read a 32 bit word from an address */
uint32_t read_reg_32(uint32_t address);

#endif
