#include <stdio.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );
void SetupTimerCTC2Interrupt( void );

unsigned long int c_Task1 = 0;
unsigned long int c_Task2 = 0;
unsigned long int f_Task1 = 0;
unsigned long int f_Task2 = 0;
unsigned long int c_Task3 = 0;
unsigned long int c_Idle = 0;


const unsigned char data_SIOB[]={
  (unsigned char) (&SIOB_C),
  sizeof(data_SIOB)-2,
             0x18,            /* channel reset*/
             0x14,0x4f,       /*0b01001111, cl.rate,par. enable X16 (115.2kbit/s) */
             0x03,0xc1,       /*0b11000001, 8bit/zn.,Rec.En. */
             0x05,0x6a,       /*0b01101000, DTR off, 8bit/zn,Tr.En,RTS off */
             0x11,0x00        /* 0b00011011, */
};

static void prvSetupHardware( void )
{
    /* Serial port SIOB - standard output*/
    outir(data_SIOB);
    /* short delay 3ms at clk=9.216MHz*/
      WDTCR = 0x4e;
    _asm
    ld  b,#0x00
    djnz  .
    djnz  .
    djnz  .
    djnz  .
    djnz  .
    djnz  .
    nop
    _endasm;
    WDTCR = 0x4e;
}

void putchar(char c)
{
  critical{
  _asm
__putchar_wloop:
    in  a,(_SIOB_C)
    bit 2,a
    jr  z,__putchar_wloop
    ld  a,4(ix)
    out (_SIOB),a
  _endasm;
  WDTCR = 0x4e;
  }
}

char getchar(void)
{
  return 0x00;
}

xQueueHandle  RxQueue;

int main(void)
{
  prvSetupHardware();
  t_puts("FreeRTOS: Procedura main "__DATE__" "__TIME__"\n");
  SCRP = 0x02;
  printf("CSBR = 0x%02x\n",SCDP);
  /* Create the sample tasks. */
	t_puts("Befor xQueueCreate RxQueue");
	RxQueue = xQueueCreate(512,sizeof(unsigned int));
	t_puts("Befor xTaskCreate TASK1");
	xTaskCreate( Task1, "Task1", configMINIMAL_STACK_SIZE, (void *)10, tskIDLE_PRIORITY + 3, (xTaskHandle *)NULL);
	puts("Befor xTaskCreate TASK2");
	xTaskCreate( Task2, "Task2", configMINIMAL_STACK_SIZE, (void *)3,  tskIDLE_PRIORITY + 2, (xTaskHandle *)NULL);
	xTaskCreate( Task3, "Task3", configMINIMAL_STACK_SIZE, NULL,  tskIDLE_PRIORITY + 1, (xTaskHandle *)NULL);
  t_puts("Befor vTaskStartScheduler\n");
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/


void Task1( void *pvParameters )
{
  unsigned int tmp;
  portTickType ticks = (unsigned int)pvParameters;
  t_puts("Task1 :   ");
  SetupTimerCTC2Interrupt();
  t_puts("\n");
	while(1) {
      tmp = ('A'&0x7f)|((xTaskGetTickCount()&0x7f)<<8);
      if( xQueueSend(RxQueue,&tmp,(portTickType)0) == errQUEUE_FULL){
         t_puts("\nT1: Q full");
         f_Task1++;
      }
      else{
        putchar('A');
      }
		c_Task1++;
		vTaskDelay(ticks);
	}
}


void Task2( void *pvParameters )
{
  unsigned int tmp;
  portTickType ticks = (unsigned int)pvParameters;
  t_puts("Task2 :   \n");

	while(1) {
  //		putchar('B');
      tmp = ('B'&0x7f)|((xTaskGetTickCount()&0x7f)<<8);
      if( xQueueSend(RxQueue,&tmp,(portTickType)0) == errQUEUE_FULL){
        t_puts("\nT2: Q full");
         f_Task2++;
      }else {
        putchar('B');
      }
  	c_Task2++;
  	vTaskDelay(ticks);
	}
}


unsigned int Task3_tmp;
void Task3( void *pvParameters )
{

  t_puts("Task3  \n");
	while(1) {
      if(xQueueReceive(RxQueue,&Task3_tmp,(portTickType)1)==pdTRUE){
        t_puts("task3_item");
        printf("<%04x>\n",Task3_tmp);
      }
      else t_puts("--Task3: Q is empty--");
	}
}


void vApplicationIdleHook( void ) __naked
{
	_asm
    ei
    nop
		halt
		ret
	_endasm;
}

const unsigned char data_CTC2[] ={
  (unsigned char)(&CTC2),
  sizeof(data_CTC2)-2,
  0xa7,
  (unsigned char)(255),
  };

unsigned char timerCTC2_c2 = 0;
unsigned int tmp_CTC2;

void timerCTC2_isr(void) __naked
{
  FAST_ISR_SAVE_CONTEXT();
  timerCTC2_c2++;
  if(timerCTC2_c2>11){
    portBASE_TYPE xTaskWokenByPost;
    tmp_CTC2 = (('C'&0x7f)|((xTaskGetTickCount()&0x7f)<<8));
    xTaskWokenByPost = pdFALSE;
    xTaskWokenByPost = xQueueSendFromISR(RxQueue,&tmp_CTC2,xTaskWokenByPost);
    timerCTC2_c2=0;
    putchar('C');
  }
  FAST_ISR_RESTORE_CONTEXT();
}

void SetupTimerCTC2Interrupt( void )
{
/* Setup of isr vector */
  t_puts("CTC2_start\n");
  outir(data_CTC2);

#pragma preproc_asm-
    _asm
	.area	_HEADER (ABS,OVR)
	.org  0x84               /* LSB adresu wektora dla przerwañ od CTC */
_vect_timer2_isr::	.dw   _timerCTC2_isr
	.area _HOME(REL)
    _endasm;
#pragma preproc_asm+
}

