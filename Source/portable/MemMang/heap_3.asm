;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.7.3 #4905 (Aug 31 2007) (MINGW32)
; This file was generated Wed Oct 17 12:13:37 2007
;--------------------------------------------------------
	.module heap_3
	.optsdcc -mz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _pvPortMalloc
	.globl _vPortFree
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
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
_WDTMR	=	0x00f0
_WDTCR	=	0x00f1
_IPR	=	0x00f2
_SCRP	=	0x00ee
_SCDP	=	0x00ef
;--------------------------------------------------------
;  ram data
;--------------------------------------------------------
	.area _DATA
;--------------------------------------------------------
; overlayable items in  ram 
;--------------------------------------------------------
	.area _OVERLAY
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;/FreeRTOS/Source/portable/MemMang/heap_3.c:55: void *pvPortMalloc( size_t xWantedSize )
;	genLabel
;	genFunction
;	---------------------------------
; Function pvPortMalloc
; ---------------------------------
_pvPortMalloc_start::
_pvPortMalloc:
	push	ix
	ld	ix,#0
	add	ix,sp
;/FreeRTOS/Source/portable/MemMang/heap_3.c:59: vTaskSuspendAll();
;	genCall
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
	call	_vTaskSuspendAll
;/FreeRTOS/Source/portable/MemMang/heap_3.c:61: pvReturn = malloc( xWantedSize );
;	genIpush
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
;	AOP_STK for 
	ld	l,4(ix)
	ld	h,5(ix)
	push	hl
;	genCall
	call	_malloc
;	genAssign
;	(registers are the same)
;/FreeRTOS/Source/portable/MemMang/heap_3.c:63: xTaskResumeAll();
;	genCall
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 1 deSending: 0
	ex	(sp),hl
	call	_xTaskResumeAll
	pop	bc
;/FreeRTOS/Source/portable/MemMang/heap_3.c:65: return pvReturn;
;	genRet
; Dump of IC_LEFT: type AOP_REG size 2
;	 reg = bc
	ld	l,c
	ld	h,b
;	genLabel
;	genEndFunction
	pop	ix
	ret
_pvPortMalloc_end::
;/FreeRTOS/Source/portable/MemMang/heap_3.c:69: void vPortFree( void *pv )
;	genLabel
;	genFunction
;	---------------------------------
; Function vPortFree
; ---------------------------------
_vPortFree_start::
_vPortFree:
	push	ix
	ld	ix,#0
	add	ix,sp
;/FreeRTOS/Source/portable/MemMang/heap_3.c:71: if( pv )
;	genIfx
;	AOP_STK for 
	ld	a,4(ix)
	or	a,5(ix)
	jr	Z,00103$
;/FreeRTOS/Source/portable/MemMang/heap_3.c:73: vTaskSuspendAll();
;	genCall
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
	call	_vTaskSuspendAll
;/FreeRTOS/Source/portable/MemMang/heap_3.c:75: free( pv );
;	genIpush
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
;	AOP_STK for 
	ld	l,4(ix)
	ld	h,5(ix)
	push	hl
;	genCall
	call	_free
	pop	af
;/FreeRTOS/Source/portable/MemMang/heap_3.c:77: xTaskResumeAll();
;	genCall
; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
	call	_xTaskResumeAll
;	genLabel
00103$:
;	genEndFunction
	pop	ix
	ret
_vPortFree_end::
	.area _CODE
	.area _CABS
