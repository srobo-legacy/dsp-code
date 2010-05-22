	.file	"/tmp/llvm_PbhHSP/start.bc"


	.globl	main
	main:
		mvk	.S1	0,	A4
		bnop	.S2	B3,	5

	.align 5
	.globl run
; Stack frame:
; -0		Previous FP
; -4		Return address
; -8		NODE_EnvPtr
; -C -> -18	Message scratchpad
; -18		free
	run:
; Enter
		stw	.D2T1	A15,	*B15
		mv	.S1X	B15,	A15
		stw	.D2T2	B3,	*-B15(4)
		sub	.D2	B15,	0x18,	B15
		stw	.D1T1	A4,	*-A15(8)
; Setup first node message - all zeros
		mvk	.S1	0,	A0
		sub	.D1	A15,	0x14,	A6	; Message scratchpad
		stw	.D1T1	A0,	*A6		; Fill it with zeros
		stw	.D1T1	A0,	*+A6(4)
		stw	.D1T1	A0,	*+A6(8)
		mvk	.S2	0,		B4
		mvk	.S2	-1,		B6
		; node ptr conveniently stays in A4
		callp	.S2	_NODE_putMsg

; Leave
		ldw	.D1T2	*-A15(4),	B3
		ldw	.D1T1	*A15,		A0
		nop	.S1	5			; Time for B3 to load
		mv	.S1	A0,	A15
		add	.D2	0x18,	B15,	B15
		mvk	.S1	0,	A4
		bnop	.S2	B3,	5
		
.align 5
	.section	.data,""
badgers:
	.asciz "I'M COVERED IN BEES"


	.section	.dcd_register,""
	.align	5
	.type dcd_register,#object
	.size dcd_register,39
dcd_register:
	.asciz	"FACEBEE5_FACE_BEE5_FACE_BEE500000000:0"
	.align 4
ponies:
	.asciz "I'M COVERED IN BEES"


	.section	.FACEBEE5_FACE_BEE5_FACE_BEE500000000,""
	.align	5
	.type death,#object
	.size death,224
death:
;	.asciz	"1024,FACEBEE5_FACE_BEE5_FACE_BEE500000000,nodename,1,0,1024,512,128,3072,0,0,0,0,0,0,1024,1024,0,10,1,1,7d0H,main,run,main,0,32768,bees,1,ff3f3f3fH,ff3f3f3fH,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,stackSeg,"

	.asciz	"1024,FACEBEE5_FACE_BEE5_FACE_BEE500000000,PING_TI,1,0,1024,512,128,3072,5,3,1000,100,10,1,1024,16,0,2,0,0,7d0H,main,run,main,0,32768,iAlgName,1,ff3f3f3fH,ff3f3f3fH,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,stackSegName,"
