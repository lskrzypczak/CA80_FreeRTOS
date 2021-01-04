/*
	FreeRTOS.org V4.5.0 - Copyright (C) 2003-2007 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

	***************************************************************************
	See http://www.FreeRTOS.org for documentation, latest information, license
	and contact details.  Please ensure to read the configuration and relevant
	port sections of the online documentation.

	Also see http://www.SafeRTOS.com for an IEC 61508 compliant version along
	with commercial development and support options.
	***************************************************************************
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *
 *	Compiler: SDCC
 *	Z80 (Z84C15)
 *
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR		     char
#define portFLOAT		     float
#define portDOUBLE		   double
#define portLONG		     long
#define portSHORT		     short
#define portSTACK_TYPE	 unsigned int
#define portBASE_TYPE	   int
/* Not supported in the SDCC  */
#define inline
#define volatile

#if ( configUSE_16_BIT_TICKS == 1 )
	typedef unsigned portSHORT portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffff
#else
	typedef unsigned portLONG portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffffffff
#endif
/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/
/* Critical section management. */
#pragma preproc_asm-
#define portENTER_CRITICAL()  _asm
	   ld   a,i
	   push af
	   di
	_endasm
#pragma preproc_asm+

#pragma preproc_asm-
#define portEXIT_CRITICAL()  _asm
	   pop  af
	   di
	   jp   po,  .+4
     ei
  _endasm
#pragma preproc_asm+


#pragma preproc_asm-
#define portDISABLE_INTERRUPTS() _asm
    di
  _endasm
#pragma preproc_asm+

#pragma preproc_asm-
#define portENABLE_INTERRUPTS()  _asm
    ei
  _endasm
#pragma preproc_asm+

/*-----------------------------------------------------------*/
/*
 * Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.
 */
 
#ifdef RTOS_Z84C15_PIOB_Trace
#define portSAVE_CONTEXT()  {__portSAVE_CONTEXT();_trace_set_OS();}
#define portSAVE_CONTEXT_from_ISR() {__portSAVE_CONTEXT_from_ISR();_trace_set_OS();}
#define portRESTORE_CONTEXT()   {_trace_clr_OS(); __portRESTORE_CONTEXT();}
#define portRESTORE_CONTEXT_from_ISR()   {_trace_clr_OS(); __portRESTORE_CONTEXT_from_ISR();}
#else
#define portSAVE_CONTEXT() __portSAVE_CONTEXT()
#define portSAVE_CONTEXT_from_ISR() __portSAVE_CONTEXT_from_ISR()
#define portRESTORE_CONTEXT() __portRESTORE_CONTEXT()
#define portRESTORE_CONTEXT_from_ISR() __portRESTORE_CONTEXT_from_ISR()
#endif


#ifdef RTOS_Z84C15_PIOB_Trace
#define _trace_set_OS() PIOB=0x80
#define _trace_clr_OS() PIOB=0x00
#endif


#pragma preproc_asm-
#define __portSAVE_CONTEXT()  _asm
             .globl  _pxCurrentTCB
             push  af
             ld    a,i
             di
             push  af
             push  hl
             push  bc
             push  de
             push  ix
             push  iy
   					 ld		 ix,(_pxCurrentTCB)
             ld    hl,#0
             add   hl,sp
             ld    0(ix),l
             ld    1(ix),h
	_endasm;

#pragma preproc_asm+

#pragma preproc_asm-
#define __portSAVE_CONTEXT_from_ISR()  _asm
             .globl  _pxCurrentTCB
             push  af
             ld    a,#0x7f      ;
             inc   a            ; set P/V
             push  af
             push  hl
             push  bc
             push  de
             push  ix
             push  iy
   					 ld		 ix,(_pxCurrentTCB)
             ld    hl,#0
             add   hl,sp
             ld    0(ix),l
             ld    1(ix),h
	_endasm
#pragma preproc_asm+



/*
 * Opposite to portSAVE_CONTEXT().
 */

#pragma preproc_asm-
#define __portRESTORE_CONTEXT() _asm
             .globl  _pxCurrentTCB
   					 ld		 ix,(_pxCurrentTCB)
             ld    l,0(ix)
             ld    h,1(ix)
             ld    sp,hl
             pop   iy
             pop   ix
             pop   de
             pop   bc
             pop   hl
             pop   af
             jp    po,.+4
             ei
             pop   af
             ret
  _endasm
#pragma preproc_asm+

#pragma preproc_asm-
#define __portRESTORE_CONTEXT_from_ISR() _asm
             .globl  _pxCurrentTCB
   					 ld		 ix,(_pxCurrentTCB)
             ld    l,0(ix)
             ld    h,1(ix)
             ld    sp,hl
             pop   iy
             pop   ix
             pop   de
             pop   bc
             pop   hl
             pop   af
             jp    po,.+4
             ei
             pop   af
             reti
  _endasm
#pragma preproc_asm+

/*-----------------------------------------------------------*/

#pragma preproc_asm-
#define FAST_ISR_SAVE_CONTEXT()  _asm
             ex    af,af'
             exx
             push  ix
             push  iy
	_endasm
#pragma preproc_asm+

#pragma preproc_asm-
#define FAST_ISR_RESTORE_CONTEXT() _asm
             pop   iy
             pop   ix
             exx
             ex    af,af'
             ei
             reti
  _endasm
#pragma preproc_asm+

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_RATE_MS			( ( portTickType ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			3
#define TIMER_VECTOR				0x54

/*-----------------------------------------------------------*/

/* Kernel utilities. */
extern void vPortYield( void ) __naked;
#define portYIELD()					vPortYield()


/*-----------------------------------------------------------*/

/* Compiler specifics. */
//#define inline
#pragma preproc_asm-
#define portNOP()				_asm
			nop
			_endasm
#pragma preproc_asm+


/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

void outir(void *ptr) __naked;

#define t_puts(a) puts(a)   /* dla celów sleczenia */



#pragma preproc_asm +
#pragma sdcc_hash +
#define SetIntVector(VECT,PROC) \
  _asm                          \
  .area _HEADER(ABS)            \
	.org  0x80+#VECT              \
  .dw   #PROC                   \
  .area _HOME                   \
  _endasm
#pragma preproc_asm +
#pragma sdcc_hash -




#endif /* PORTMACRO_H */

