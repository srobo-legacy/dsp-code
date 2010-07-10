; SR ISR, responsible for dumping everything in register state onto the stack
; and then passing it onwards to some C. Note that TIC64X+ doesn't have a
; system stack, the numpties, so there's no guarentee we won't crash and burn
; from stack overflow at this point.

        .text
        .globl  sr_hacks_isr
        .align  5
        .type   sr_hacks_isr,@function
sr_hacks_isr:
; State at this point in execution is that B0 gets stored to stack, used to push
; the #intr ID to B15, and then gets restored. So we enter with state intact and
; the #intr id on the stack.

; point SP past isr id.
		sub		B15,	4,	B15
; Save SP immediately for easy calculation of what it was later.
		stw		B15,	*B15
; Free up B0.
		stw		B0,	*-B15(4)
; Generate some space on the stack
		mvk		0x180,	B0
		sub		B15,	B0,	B15
; Start storing everything else
		stw		B1,	*B15(0x17C)
		stw		B2,	*B15(0x178)
		stw		B3,	*B15(0x174)
		stw		B4,	*B15(0x170)
		stw		B5,	*B15(0x16C)
		stw		B6,	*B15(0x168)
		stw		B7,	*B15(0x164)
		stw		B8,	*B15(0x160)
		stw		B9,	*B15(0x15C)
		stw		B10,	*B15(0x158)
		stw		B11,	*B15(0x154)
		stw		B12,	*B15(0x150)
		stw		B13,	*B15(0x14C)
		stw		B14,	*B15(0x148)
		stw		B15,	*B15(0x144)
		stw		B16,	*B15(0x140)
		stw		B17,	*B15(0x13C)
		stw		B18,	*B15(0x138)
		stw		B19,	*B15(0x134)
		stw		B20,	*B15(0x130)
		stw		B21,	*B15(0x12C)
		stw		B22,	*B15(0x128)
		stw		B23,	*B15(0x124)
		stw		B24,	*B15(0x120)
		stw		B25,	*B15(0x11C)
		stw		B26,	*B15(0x118)
		stw		B27,	*B15(0x114)
		stw		B28,	*B15(0x110)
		stw		B29,	*B15(0x10C)
		stw		B30,	*B15(0x108)
		stw		B31,	*B15(0x104)
		stw		A0,	*B15(0x100)
		stw		A1,	*B15(0xFC)
		stw		A2,	*B15(0xF8)
		stw		A3,	*B15(0xF4)
		stw		A4,	*B15(0xF0)
		stw		A5,	*B15(0xEC)
		stw		A6,	*B15(0xE8)
		stw		A7,	*B15(0xE4)
		stw		A8,	*B15(0xE0)
		stw		A9,	*B15(0xDC)
		stw		A10,	*B15(0xD8)
		stw		A11,	*B15(0xD4)
		stw		A12,	*B15(0xD0)
		stw		A13,	*B15(0xCC)
		stw		A14,	*B15(0xC8)
		stw		A15,	*B15(0xC4)
		stw		A16,	*B15(0xC0)
		stw		A17,	*B15(0xBC)
		stw		A18,	*B15(0xB8)
		stw		A19,	*B15(0xB4)
		stw		A20,	*B15(0xB0)
		stw		A21,	*B15(0xAC)
		stw		A22,	*B15(0xA8)
		stw		A23,	*B15(0xA4)
		stw		A24,	*B15(0xA0)
		stw		A25,	*B15(0x9C)
		stw		A26,	*B15(0x98)
		stw		A27,	*B15(0x94)
		stw		A28,	*B15(0x90)
		stw		A29,	*B15(0x8C)
		stw		A30,	*B15(0x88)
		stw		A31,	*B15(0x84)

; We can now overwrite any GPR register. Start storing control registers, by
; order of address if no better reason

		mvc		AMR,	B0
		stw		B0,	*B15(0x0)
		mvc		CSR,	B0
		stw		B0,	*B15(0x4)
		mvc		IFR,	B0
		stw		B0,	*B15(0x8)
		mvc		ICR,	B0
		stw		B0,	*B15(0xC)
		mvc		IER,	B0
		stw		B0,	*B15(0x10)
		mvc		ISTP,	B0
		stw		B0,	*B15(0x14)
		mvc		IRP,	B0
		stw		B0,	*B15(0x18)
		mvc		NRP,	B0
		stw		B0,	*B15(0x1C)
; There is no ctrl 8 or 9
		mvc		TSCL,	B0
		stw		B0,	*B15(0x28)
		mvc		TSCH,	B0
		stw		B0,	*B15(0x2C)
; No 12
		mvc		ILC,	B0
		stw		B0,	*B15(0x34)
		mvc		RILC,	B0
		stw		B0,	*B15(0x38)
		mvc		REP,	B0
		stw		B0,	*B15(0x3C)
		mvc		PCE1,	B0
		stw		B0,	*B15(0x40)
		mvc		DNUM,	B0
		stw		B0,	*B15(0x44)
; No 18, 19, 20
		mvc		SSR,	B0
		stw		B0,	*B15(0x54)
		mvc		GPLYA,	B0
		stw		B0,	*B15(0x58)
		mvc		GPLYB,	B0
		stw		B0,	*B15(0x5C)
		mvc		GFPGFR,	B0
		stw		B0,	*B15(0x60)
		mvc		DIER,	B0
		stw		B0,	*B15(0x64)
		mvc		TSR,	B0
		stw		B0,	*B15(0x68)
		mvc		ISTR,	B0
		stw		B0,	*B15(0x6C)
		mvc		NTSR,	B0
		stw		B0,	*B15(0x70)
		mvc		ECR,	B0
		stw		B0,	*B15(0x74)
		mvc		ECR,	B0
		stw		B0,	*B15(0x78)
; No 30
		mvc		IERR,	B0
		stw		B0,	*B15(0x80)

; And that in fact, is it. Now place ptr to that immense struct of registers
; in A4 and jump to some C code. We do *not* expect to return.

		mv		B15,	A4
		b	.S2	sr_trap
		nop		5

