	;; Generic crt0.s for a Z84C15
        .module crt0
       	.globl	_main
       	.globl  s__HEAP_END
       	.globl  __ivector
       	.globl  __STACK
      	.globl  s__DATA

_WDTMR	=	0x00f0
_WDTCR	=	0x00f1
_SCRP	=	0x00ee
_SCDP	=	0x00ef

_CTC	=	0x0010
_CTC0	=	0x0010
_CTC1	=	0x0011
_CTC2	=	0x0012
_CTC3	=	0x0013
_SIO	=	0x0018
_SIOA	=	0x0018
_SIOA_C	=	0x0019
_SIOB	=	0x001a
_SIOB_C	=	0x001b
_PIO	=	0x001c
_PIOA	=	0x001c
_PIOA_C	=	0x001d
_PIOB	=	0x001e
_PIOB_C	=	0x001f


	.area	_HEADER (ABS,OVR)
	;; Reset vector
	.org 	0
	jp	init

	.org	0x08
	reti
	.org	0x10
	reti
	.org	0x18
	reti
	.org	0x20
	reti
	.org	0x28
	reti
	.org	0x30
	reti
	.org	0x38
	reti
	.org	0x66
	retn
	
	; Interrupt table
	
	.org	0x80
__ivector::
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
;-----------
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
;-----------
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
;-----------
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
  .dw 0x0008
;-----------


	;; Ordering of segments for the linker.
	.area	_HOME
init:
; Z84C15 specific registers
;
; Memory MAP:
; ROM: CS0: 0x0000...s__DATA-1
; RAM: CS1: s__DATA...0xffff

  ld  a,#0x02           ; point to CSBR
  out (_SCRP),a
  ld  a,#>s__DATA
  sra a
  sra a
  sra a
  sra a
  dec a
  or  a,#0xf0
  out (_SCDP),a
 ;
  ld  a,#0x03            ; point to MCR
  out (_SCRP),a
  ld  a,#0x07            ; CS0, CS1, CRC-32 enable
  out (_SCDP),a
;
  ld  a,#0x00            ; point to CSBR - not programmed
  out (_SCRP),a
  ld  a,#0xff
  out (_SCDP),a
;
	ld	a,#0x4E            ; watch-dog
	out	(_WDTCR),a
;
; Stack at the top of memory.
	ld  sp,#__STACK
; Interrupt mode 2
  ld  a,#>__ivector
  ld  i,a
  im  2
; clear interrupts
;ctc
  ld  a,#0x03
  out (_CTC0),a
  out (_CTC1),a
  out (_CTC2),a
  out (_CTC3),a
;pio
  ld  a,#0x07
  out (_PIOA_C),a
  out (_PIOB_C),a

; fill heap 0xff
  ld  de,#__sdcc_heap_start+1
  ld  hl,#__sdcc_heap_end
  scf
  ccf
  sbc hl,de
  push hl
  pop bc
  ld  hl,#__sdcc_heap_start
  ld  (hl),#0xff
  ldir
;
	ld	a,#0x4E            ; watch-dog
	out	(_WDTCR),a
;
  call    gsinit
	ld	a,#0x4E            ; watch-dog
	out	(_WDTCR),a
;
	call	_main
1$:
	halt
	jr	1$
	.area	_CODE
  .area _GSINIT
  .area _GSFINAL

	.area	_DATA
  .area _BSS
  .area _OVERLAY
  .area _HEAP
__sdcc_heap_start::
  .area _HEAP_END
__sdcc_heap_end::
        .ds     1

        .area   _CODE

        .area   _GSINIT
gsinit::

        .area   _GSFINAL
        ret
        
        ;; Stubs that hook the heap in
        .globl  __sdcc_heap_init

        .area   _GSINIT
        call    __sdcc_heap_init

        .area   _HEAP

        .area   _HEAP_END

        
