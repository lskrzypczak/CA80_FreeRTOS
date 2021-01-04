                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.7.3 #4905 (Aug 31 2007) (MINGW32)
                              4 ; This file was generated Wed Oct 17 12:13:37 2007
                              5 ;--------------------------------------------------------
                              6 	.module heap_3
                              7 	.optsdcc -mz80
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _pvPortMalloc
                             13 	.globl _vPortFree
                             14 ;--------------------------------------------------------
                             15 ; special function registers
                             16 ;--------------------------------------------------------
                    0010     17 _CTC	=	0x0010
                    0010     18 _CTC0	=	0x0010
                    0011     19 _CTC1	=	0x0011
                    0012     20 _CTC2	=	0x0012
                    0013     21 _CTC3	=	0x0013
                    0018     22 _SIO	=	0x0018
                    0018     23 _SIOA	=	0x0018
                    0019     24 _SIOA_C	=	0x0019
                    001A     25 _SIOB	=	0x001a
                    001B     26 _SIOB_C	=	0x001b
                    001C     27 _PIO	=	0x001c
                    001C     28 _PIOA	=	0x001c
                    001D     29 _PIOA_C	=	0x001d
                    001E     30 _PIOB	=	0x001e
                    001F     31 _PIOB_C	=	0x001f
                    00F0     32 _WDTMR	=	0x00f0
                    00F1     33 _WDTCR	=	0x00f1
                    00F2     34 _IPR	=	0x00f2
                    00EE     35 _SCRP	=	0x00ee
                    00EF     36 _SCDP	=	0x00ef
                             37 ;--------------------------------------------------------
                             38 ;  ram data
                             39 ;--------------------------------------------------------
                             40 	.area _DATA
                             41 ;--------------------------------------------------------
                             42 ; overlayable items in  ram 
                             43 ;--------------------------------------------------------
                             44 	.area _OVERLAY
                             45 ;--------------------------------------------------------
                             46 ; external initialized ram data
                             47 ;--------------------------------------------------------
                             48 ;--------------------------------------------------------
                             49 ; global & static initialisations
                             50 ;--------------------------------------------------------
                             51 	.area _HOME
                             52 	.area _GSINIT
                             53 	.area _GSFINAL
                             54 	.area _GSINIT
                             55 ;--------------------------------------------------------
                             56 ; Home
                             57 ;--------------------------------------------------------
                             58 	.area _HOME
                             59 	.area _HOME
                             60 ;--------------------------------------------------------
                             61 ; code
                             62 ;--------------------------------------------------------
                             63 	.area _CODE
                             64 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:55: void *pvPortMalloc( size_t xWantedSize )
                             65 ;	genLabel
                             66 ;	genFunction
                             67 ;	---------------------------------
                             68 ; Function pvPortMalloc
                             69 ; ---------------------------------
   0000                      70 _pvPortMalloc_start::
   0000                      71 _pvPortMalloc:
   478C DD E5                72 	push	ix
   478E DD 21 00 00          73 	ld	ix,#0
   4792 DD 39                74 	add	ix,sp
                             75 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:59: vTaskSuspendAll();
                             76 ;	genCall
                             77 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
   4794 CD F2 2E             78 	call	_vTaskSuspendAll
                             79 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:61: pvReturn = malloc( xWantedSize );
                             80 ;	genIpush
                             81 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
                             82 ;	AOP_STK for 
   4797 DD 6E 04             83 	ld	l,4(ix)
   479A DD 66 05             84 	ld	h,5(ix)
   479D E5                   85 	push	hl
                             86 ;	genCall
   479E CD 0C 49             87 	call	_malloc
                             88 ;	genAssign
                             89 ;	(registers are the same)
                             90 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:63: xTaskResumeAll();
                             91 ;	genCall
                             92 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 1 deSending: 0
   47A1 E3                   93 	ex	(sp),hl
   47A2 CD 09 2F             94 	call	_xTaskResumeAll
   47A5 C1                   95 	pop	bc
                             96 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:65: return pvReturn;
                             97 ;	genRet
                             98 ; Dump of IC_LEFT: type AOP_REG size 2
                             99 ;	 reg = bc
   47A6 69                  100 	ld	l,c
   47A7 60                  101 	ld	h,b
                            102 ;	genLabel
                            103 ;	genEndFunction
   47A8 DD E1               104 	pop	ix
   47AA C9                  105 	ret
   001F                     106 _pvPortMalloc_end::
                            107 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:69: void vPortFree( void *pv )
                            108 ;	genLabel
                            109 ;	genFunction
                            110 ;	---------------------------------
                            111 ; Function vPortFree
                            112 ; ---------------------------------
   001F                     113 _vPortFree_start::
   001F                     114 _vPortFree:
   47AB DD E5               115 	push	ix
   47AD DD 21 00 00         116 	ld	ix,#0
   47B1 DD 39               117 	add	ix,sp
                            118 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:71: if( pv )
                            119 ;	genIfx
                            120 ;	AOP_STK for 
   47B3 DD 7E 04            121 	ld	a,4(ix)
   47B6 DD B6 05            122 	or	a,5(ix)
   47B9 28 11               123 	jr	Z,00103$
                            124 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:73: vTaskSuspendAll();
                            125 ;	genCall
                            126 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
   47BB CD F2 2E            127 	call	_vTaskSuspendAll
                            128 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:75: free( pv );
                            129 ;	genIpush
                            130 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
                            131 ;	AOP_STK for 
   47BE DD 6E 04            132 	ld	l,4(ix)
   47C1 DD 66 05            133 	ld	h,5(ix)
   47C4 E5                  134 	push	hl
                            135 ;	genCall
   47C5 CD 0A 48            136 	call	_free
   47C8 F1                  137 	pop	af
                            138 ;/FreeRTOS/Source/portable/MemMang/heap_3.c:77: xTaskResumeAll();
                            139 ;	genCall
                            140 ; _saveRegsForCall: sendSetSize: 0 deInUse: 0 bcInUse: 0 deSending: 0
   47C9 CD 09 2F            141 	call	_xTaskResumeAll
                            142 ;	genLabel
   0040                     143 00103$:
                            144 ;	genEndFunction
   47CC DD E1               145 	pop	ix
   47CE C9                  146 	ret
   0043                     147 _vPortFree_end::
                            148 	.area _CODE
                            149 	.area _CABS
