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
#pragma codeseg HOME
#include <stdlib.h>

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the Z80 port.
 *----------------------------------------------------------*/

/* Standard includes. */
#include <string.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/*
typedef struct{
  unsigned char len;
//  unsigned char *port;
  unsigned char cw;
  unsigned char div;
  void *isr;
} CTC_struct;
*/

/*-----------------------------------------------------------*/

/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
typedef void tskTCB;
extern volatile tskTCB * volatile pxCurrentTCB;

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

static void prvSetupTimerInterrupt( void );
/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void * pvParameters )
{
	/* Place the parameter on the stack in the expected location. */
	*pxTopOfStack--= ( portSTACK_TYPE ) pvParameters;

	/* Place the task return address on stack. Not used */
	*pxTopOfStack-- = ( portSTACK_TYPE ) 0x000000;
	
	/* The start of the task code will be popped off the stack last, so place
	it on first. */
	*pxTopOfStack-- = ( portSTACK_TYPE ) pxCode;

	/* Now the registers. */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0xAFFA;	/* AF  */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0x0404;	/* IFF=1  */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0x9889;	/* HL  */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0xBCCB;	/* BC  */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0xDEED;	/* DE  */
 *pxTopOfStack-- = ( portSTACK_TYPE ) 0x1122;	/* IX  */
 *pxTopOfStack   = ( portSTACK_TYPE ) 0x3344;	/* IY  */

	return pxTopOfStack;

}
/*-----------------------------------------------------------*/

portBASE_TYPE xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick. */
	prvSetupTimerInterrupt();

	/* Restore the context of the first task that is going to run. */
	portRESTORE_CONTEXT();

	/* Should not get here. */
	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the AVR port will get stopped.  If required simply
	disable the tick interrupt here. */
	puts("\n ************vPortEndScheduler ********************\n");
}
/*-----------------------------------------------------------*/

/*
 * Manual context switch.  The first thing we do is save the registers so we
 * can use a naked attribute.
 */
void vPortYield( void )  __naked
{	
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();
}

/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to 
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void vPortYieldFromTick( void )
{	
	portSAVE_CONTEXT();
	vTaskIncrementTick();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void timer_isr(void) __naked
{	
#if configUSE_PREEMPTION == 1
	/*
 	 * Tick ISR for preemptive scheduler.  We can use a naked attribute as
	 * the context is saved at the start of vPortYieldFromTick().  The tick
	 * count is incremented after the context is saved.
	 */
    portSAVE_CONTEXT_from_ISR();
    WDTCR = 0x4e;
    vTaskIncrementTick();
    vTaskSwitchContext();
    portRESTORE_CONTEXT_from_ISR();
#else
	/*
	 * Tick ISR for the cooperative scheduler.  All this does is increment the
	 * tick count.  We don't need to switch context, this can only be done by
	 * manual calls to taskYIELD();
	 */
    portSAVE_CONTEXT_from_ISR();
  	vTaskIncrementTick();
    portRESTORE_CONTEXT_from_ISR();
#endif
}

/*
    Fast output to port:
    ptr[0]              - port address
    ptr[1]              - number of bytes
    ptr[2..2+ptr[1]-1]  - bytes to outpyy

*/

void outir(void *ptr) __naked
{
  _asm
    ld   a,i
    di
    ex    af,af'
    exx
    pop   de
    pop   hl
    push  hl          ; *ptr
    push  de
    ld    c,(hl)
    inc   hl
    ld    b,(hl)
    inc   hl
    otir
    exx
    ex    af,af'
    ret   po
    ei
    ret
  _endasm;
}


/*
 * Setup timer CTC0.
 */
const unsigned char data_CTC0[] ={
  (unsigned char)(&CTC0),
  sizeof(data_CTC0)-2,
  0xa7,
  (unsigned char)((configCPU_CLOCK_HZ/256)/configTICK_RATE_HZ),
  0x80  /* LSB adresu wektora dla przerwañ od CTC */
  };
#ifdef RTOS_Z84C15_PIOB_Trace
const unsigned char data_PIOB[]={
  (unsigned char)(&PIOB_C),
  sizeof(data_PIOB)-2,
  0x07,
  0xcf,
  0x00       /* all outputs */
};
#endif

static void prvSetupTimerInterrupt( void )
{
/* Setup of isr vector */
  outir(data_CTC0);
  #ifdef RTOS_Z84C15_PIOB_Trace
  outir(data_PIOB);
  #endif

#pragma preproc_asm-
    _asm
	.area	_HEADER (ABS,OVR)
	.org  0x80               /* LSB adresu wektora dla przerwañ od CTC */
_vect_timer_isr::	.dw   _timer_isr
	.area _HOME(REL)
    _endasm;
#pragma preproc_asm+
}
/*-----------------------------------------------------------*/


