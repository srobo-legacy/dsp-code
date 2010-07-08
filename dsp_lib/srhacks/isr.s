; SR ISR, responsible for dumping everything in register state onto the stack
; and then passing it onwards to some C. Note that TIC64X+ doesn't have a
; system stack, the numpties, so there's no guarentee we won't crash and burn
; from stack overflow at this point.

        .text
        .globl  srhacks_isr
        .align  5
        .type   srhacks_isr,@function
srhacks_isr:
; State at this point in execution is that B0 gets stored to stack, used to push
; the #intr ID to B15, and then gets restored. So we enter with state intact and
; the #intr id on the stack.

; point SP past isr id.
		sub		B15,	4,	B15
; Save SP immediately for easy calculation of what it was later.
		stw		B15,	*B15--
		stw		B0,	*B15--
		stw		B1,	*B15--
		stw		B2,	*B15--
		stw		B3,	*B15--
		stw		B4,	*B15--
		stw		B5,	*B15--
		stw		B6,	*B15--
		stw		B7,	*B15--
		stw		B8,	*B15--
		stw		B9,	*B15--
		stw		B10,	*B15--
		stw		B11,	*B15--
		stw		B12,	*B15--
		stw		B13,	*B15--
		stw		B14,	*B15--
		stw		B15,	*B15--
; never mind about the replication, this makes lookups easier
		stw		B16,	*B15--
		stw		B17,	*B15--
		stw		B18,	*B15--
		stw		B19,	*B15--
		stw		B20,	*B15--
		stw		B21,	*B15--
		stw		B22,	*B15--
		stw		B23,	*B15--
		stw		B24,	*B15--
		stw		B25,	*B15--
		stw		B26,	*B15--
		stw		B27,	*B15--
		stw		B28,	*B15--
		stw		B29,	*B15--
		stw		B30,	*B15--
		stw		B31,	*B15--
		stw		A0,	*B15--
		stw		A1,	*B15--
		stw		A2,	*B15--
		stw		A3,	*B15--
		stw		A4,	*B15--
		stw		A5,	*B15--
		stw		A6,	*B15--
		stw		A7,	*B15--
		stw		A8,	*B15--
		stw		A9,	*B15--
		stw		A10,	*B15--
		stw		A11,	*B15--
		stw		A12,	*B15--
		stw		A13,	*B15--
		stw		A14,	*B15--
		stw		A15,	*B15--
		stw		A16,	*B15--
		stw		A17,	*B15--
		stw		A18,	*B15--
		stw		A19,	*B15--
		stw		A20,	*B15--
		stw		A21,	*B15--
		stw		A22,	*B15--
		stw		A23,	*B15--
		stw		A24,	*B15--
		stw		A25,	*B15--
		stw		A26,	*B15--
		stw		A27,	*B15--
		stw		A28,	*B15--
		stw		A29,	*B15--
		stw		A30,	*B15--
		stw		A31,	*B15--

; We can now overwrite any GPR register. Start storing control registers, by
; order of address if no better reason

		mvc		AMR,	B0
		stw		B0,	*B15--
		mvc		CSR,	B0
		stw		B0,	*B15--
		mvc		IFR,	B0
		stw		B0,	*B15--
		mvc		ICR,	B0
		stw		B0,	*B15--
		mvc		IER,	B0
		stw		B0,	*B15--
		mvc		ISTP,	B0
		stw		B0,	*B15--
		mvc		IRP,	B0
		stw		B0,	*B15--
		mvc		NRP,	B0
		stw		B0,	*B15--
; There is no ctrl 8 or 9
		sub		B15,	8,	B15
		mvc		TSCL,	B0
		stw		B0,	*B15--
		mvc		TSCH,	B0
		stw		B0,	*B15--
; No 12
		sub		B15,	4,	B15
		mvc		ILC,	B0
		stw		B0,	*B15--
		mvc		RILC,	B0
		stw		B0,	*B15--
		mvc		REP,	B0
		stw		B0,	*B15--
		mvc		PCE1,	B0
		stw		B0,	*B15--
		mvc		DNUM,	B0
		stw		B0,	*B15--
; No 18, 19, 20
		sub		B15, 	0xC,	B15
		mvc		SSR,	B0
		stw		B0,	*B15--
		mvc		GPLYA,	B0
		stw		B0,	*B15--
		mvc		GPLYB,	B0
		stw		B0,	*B15--
		mvc		GFPGFR,	B0
		stw		B0,	*B15--
		mvc		DIER,	B0
		stw		B0,	*B15--
		mvc		TSR,	B0
		stw		B0,	*B15--
		mvc		ISTR,	B0
		stw		B0,	*B15--
		mvc		NTSR,	B0
		stw		B0,	*B15--
		mvc		ECR,	B0
		stw		B0,	*B15--
		mvc		ECR,	B0
		stw		B0,	*B15--
; No 30
		sub		B15,	4,	B15
		mvc		IERR,	B0
		stw		B0,	*B15--

; And that in fact, is it. Now place ptr to that immense struct of registers
; in A4 and jump to some C code. We do *not* expect to return.

		add		B15,	4,	A4
		b	.S2	sr_trap
		nop		5

