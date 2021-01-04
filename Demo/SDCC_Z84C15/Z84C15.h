#ifndef SDCC_Z84C15
#define SDCC_Z84C15

/*
  Definicje rejestrów dla Z84C15


*/


/* Counter - Timer on chip*/
sfr at 0x10    CTC;
sfr at 0x10    CTC0;
sfr at 0x11    CTC1;
sfr at 0x12    CTC2;
sfr at 0x13    CTC3;

/* SIO on chip */

sfr at 0x18     SIO;
sfr at 0x18+0   SIOA;
sfr at 0x18+1   SIOA_C;
sfr at 0x18+2   SIOB;
sfr at 0x18+3   SIOB_C;

/* PIO on cip */
sfr at 0x1c     PIO;
sfr at 0x1c+0   PIOA;
sfr at 0x1c+1   PIOA_C;
sfr at 0x1c+2   PIOB;
sfr at 0x1c+3   PIOB_C;

/* Watch-dog, interrupt, system */
sfr at 0xf0     WDTMR;       /* Master Watch-dog register */
sfr at 0xf1     WDTCR;       /* Control Watch-dog register */
sfr at 0xf2     IPR;         /* Interrupt Prioruty register */
sfr at 0xee     SCRP;
sfr at 0xef     SCDP;

#endif
