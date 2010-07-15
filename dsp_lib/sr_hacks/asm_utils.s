; Some asm utility functions - see asm_utils.h for a short description.

        .text
        .globl  getsp
        .align  5
        .type   getsp,@function
getsp:
		mv		B15,	A4
		bnop		B3,	5

        .globl  getistp
        .align  5
        .type   getistp,@function
getistp:
		mvc		ISTP,	B4
		mv		B4,	A4
		; Bits 9->5 can be the pending intr number, mask out
		mvkl		0xFC00,	A0
		mvkh		0xFFFFFFFF,	A0
		and		A4,	A0,	A4
		bnop	.S2	B3,	5

        .globl  getier
        .align  5
        .type   getier,@function
getier:
		mvc		IER,	B4
		mv		B4,	A4
		bnop	.S2	B3, 	5

        .globl  setier
        .align  5
        .type   setier,@function
setier:
		mv		A4,	B4
		mvc		B4,	IER
		bnop	.S2	B3,	5

        .globl  write_reg_32
        .align  5
        .type   write_reg_32,@function
write_reg_32:
		stw	.D1T2	B4,	*A4
		bnop	.S2	B3,	5

        .globl  read_reg_32
        .align  5
        .type   read_reg_32,@function
read_reg_32:
		ldw	.D1T1	*A4,	A4
		bnop	.S2	B3,	5

