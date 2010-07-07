#ifndef _DSP_CODE_DSP_LIBS_SRHACKS_ASM_UTILS_H_
#define _DSP_CODE_DSP_LIBS_SRHACKS_ASM_UTILS_H_

#include <stdint.h>

/* Some utility functions to allow direct hardware access from C */

/* Fetch pointer to interrupt service table */
uint32_t getistp();

/* Fetch interrupt enable register */
uint32_t getier();

/* Set interrupt enable register */
void setier(uint32_t ie_mask);

/* Write a 32 bit word to an address */
void write_reg_32(uint32_t address, uint32_t value);

/* Read a 32 bit word from an address */
uint32_t read_reg_32(uint32_t address);

#endif
