/*
 *  QuarterK.cpp - Library to drive the QuarterK 16x16 Led Matrix shield
 *
 *  v1.0 Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit 
 *  http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, 
 *  San Francisco, California, 94105, USA.
 *
 */

#include "WProgram.h"
#include "QuarterK.h"

#include <avr/pgmspace.h>

#define SCK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define SS_PIN    10

byte framebuffer[32];
volatile unsigned int tick;


void QuarterK::Delay(unsigned int ms) {
	while (ms-->0) {
		unsigned int oldtick=tick;
		while(tick==oldtick);
	};
}


QuarterK::QuarterK() {
  byte tmp;
	
  // Set D0-7 as outputs for the column drivers
  // and turn all columns off
	DDRD=0xFF;
	PORTB=0x00;


  // Setup SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);


  // Enable SPI master 
  SPCR = (1<<SPE) | (1<<DORD) | (1<<MSTR);
  tmp = SPSR;
  tmp = SPDR;

	InitializeTimer1();
	}



	
	
	
	


ISR(TIMER1_COMPA_vect) {
	static byte nr=0;
	byte offset;

	tick++;

	offset=((nr+1)&0x07)*2;

	SPDR = framebuffer[17+offset];
	while (!(SPSR & (1<<SPIF)));
	SPDR = framebuffer[16+offset];
	while (!(SPSR & (1<<SPIF)));
	SPDR = framebuffer[1+offset];
	while (!(SPSR & (1<<SPIF)));
	SPDR = framebuffer[0+offset];
	while (!(SPSR & (1<<SPIF)));

	digitalWrite(nr, LOW);        // Turn off old cathodes
	digitalWrite(SS_PIN, HIGH);   // Latch shifted data for the anodes
	nr++;
	nr=nr&0x07;
	digitalWrite(nr, HIGH);       // Turn on the new cathodes
	digitalWrite(SS_PIN, LOW);
}




void QuarterK::InitializeTimer1() {
	TCCR1A = 0x00;
	TCCR1B = (_BV(WGM12) | _BV(CS11) | _BV(CS10)); //Divide by 64
	OCR1A = 249;  //Count to 250
	TIMSK1 = _BV(OCIE1A);
	sei();                                            	        // ensures that interrupts are globally enabled
}






void QuarterK::ShiftLeft(void) {
 asm volatile(
	"clc"					       	"\n\t"	// Line1
	"lds r0, framebuffer+1"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+1, r0 "     	"\n\t"
	"lds r0, framebuffer+0"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+0, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line2
	"lds r0, framebuffer+3"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+3, r0 "     	"\n\t"
	"lds r0, framebuffer+2"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+2, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line3
	"lds r0, framebuffer+5"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+5, r0 "     	"\n\t"
	"lds r0, framebuffer+4"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+4, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line4
	"lds r0, framebuffer+7"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+7, r0 "     	"\n\t"
	"lds r0, framebuffer+6"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+6, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line5
	"lds r0, framebuffer+9"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+9, r0 "     	"\n\t"
	"lds r0, framebuffer+8"        	"\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+8, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line6
	"lds r0, framebuffer+11"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+11, r0 "     	"\n\t"
	"lds r0, framebuffer+10"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+10, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line7
	"lds r0, framebuffer+13"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+13, r0 "     	"\n\t"
	"lds r0, framebuffer+12"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+12, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line8
	"lds r0, framebuffer+15"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+15, r0 "     	"\n\t"
	"lds r0, framebuffer+14"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+14, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line9
	"lds r0, framebuffer+17"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+17, r0 "     	"\n\t"
	"lds r0, framebuffer+16"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+16, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line10
	"lds r0, framebuffer+19"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+19, r0 "     	"\n\t"
	"lds r0, framebuffer+18"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+18, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line11
	"lds r0, framebuffer+21"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+21, r0 "     	"\n\t"
	"lds r0, framebuffer+20"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+20, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line12
	"lds r0, framebuffer+23"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+23, r0 "     	"\n\t"
	"lds r0, framebuffer+22"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+22, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line13
	"lds r0, framebuffer+25"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+25, r0 "     	"\n\t"
	"lds r0, framebuffer+24"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+24, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line14
	"lds r0, framebuffer+27"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+27, r0 "     	"\n\t"
	"lds r0, framebuffer+26"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+26, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line15
	"lds r0, framebuffer+29"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+29, r0 "     	"\n\t"
	"lds r0, framebuffer+28"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+28, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line16
	"lds r0, framebuffer+31"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+31, r0 "     	"\n\t"
	"lds r0, framebuffer+30"        "\n\t"
	"ror r0"						"\n\t"
    "sts framebuffer+30, r0 "     	"\n\t"
	);
}


void QuarterK::ShiftRight(void) {
 asm volatile(
	"clc"					       	"\n\t"	// Line1
	"lds r0, framebuffer+0"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+0, r0 "     	"\n\t"
	"lds r0, framebuffer+1"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+1, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line2
	"lds r0, framebuffer+2"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+2, r0 "     	"\n\t"
	"lds r0, framebuffer+3"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+3, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line3
	"lds r0, framebuffer+4"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+4, r0 "     	"\n\t"
	"lds r0, framebuffer+5"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+5, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line4
	"lds r0, framebuffer+6"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+6, r0 "     	"\n\t"
	"lds r0, framebuffer+7"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+7, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line5
	"lds r0, framebuffer+8"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+8, r0 "     	"\n\t"
	"lds r0, framebuffer+9"        	"\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+9, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line6
	"lds r0, framebuffer+10"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+10, r0 "     	"\n\t"
	"lds r0, framebuffer+11"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+11, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line7
	"lds r0, framebuffer+12"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+12, r0 "     	"\n\t"
	"lds r0, framebuffer+13"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+13, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line8
	"lds r0, framebuffer+14"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+14, r0 "     	"\n\t"
	"lds r0, framebuffer+15"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+15, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line9
	"lds r0, framebuffer+16"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+16, r0 "     	"\n\t"
	"lds r0, framebuffer+17"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+17, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line10
	"lds r0, framebuffer+18"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+18, r0 "     	"\n\t"
	"lds r0, framebuffer+19"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+19, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line11
	"lds r0, framebuffer+20"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+20, r0 "     	"\n\t"
	"lds r0, framebuffer+21"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+21, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line12
	"lds r0, framebuffer+22"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+22, r0 "     	"\n\t"
	"lds r0, framebuffer+23"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+23, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line13
	"lds r0, framebuffer+24"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+24, r0 "     	"\n\t"
	"lds r0, framebuffer+25"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+25, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line14
	"lds r0, framebuffer+26"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+26, r0 "     	"\n\t"
	"lds r0, framebuffer+27"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+27, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line15
	"lds r0, framebuffer+28"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+28, r0 "     	"\n\t"
	"lds r0, framebuffer+29"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+29, r0 "     	"\n\t"

	"clc"					       	"\n\t"	// Line16
	"lds r0, framebuffer+30"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+30, r0 "     	"\n\t"
	"lds r0, framebuffer+31"        "\n\t"
	"rol r0"						"\n\t"
    "sts framebuffer+31, r0 "     	"\n\t"
	);
}



void QuarterK::ShiftUp(void) {
	memmove(&framebuffer[0], &framebuffer[2], 30);
	framebuffer[30]=0;
	framebuffer[31]=0;
}

void QuarterK::ShiftDown(void) {
	memmove(&framebuffer[2], &framebuffer[0], 30);
	framebuffer[0]=0;
	framebuffer[1]=0;
}


void QuarterK::Clear(void) {

	// Sketch size 2484
//000001da <_ZN8QuarterK5ClearEv>:
// 1da:	e8 e0       	ldi	r30, 0x08	; 8
// 1dc:	f1 e0       	ldi	r31, 0x01	; 1
// 1de:	11 92       	st	Z+, r1
// 1e0:	81 e0       	ldi	r24, 0x01	; 1
// 1e2:	e8 32       	cpi	r30, 0x28	; 40
// 1e4:	f8 07       	cpc	r31, r24
// 1e6:	d9 f7       	brne	.-10     	; 0x1de <_ZN8QuarterK5ClearEv+0x4>
// 1e8:	08 95       	ret
 
 		byte *p;
 	byte i;
	
	p=framebuffer;
	i=32;
	do {
		*p=0;
		p++;
		i--;
	} while (i!=0);
	

/*
	// s per 1000 000 calls
	// Sketch size 2488

//000001da <_ZN8QuarterK5ClearEv>:
 //1da:	80 e2       	ldi	r24, 0x20	; 32
 //1dc:	81 50       	subi	r24, 0x01	; 1
 //1de:	e8 2f       	mov	r30, r24
 //1e0:	f0 e0       	ldi	r31, 0x00	; 0
 //1e2:	e8 5f       	subi	r30, 0xF8	; 248
 //1e4:	fe 4f       	sbci	r31, 0xFE	; 254
 //1e6:	10 82       	st	Z, r1
 //1e8:	88 23       	and	r24, r24
 //1ea:	c1 f7       	brne	.-16     	; 0x1dc <_ZN8QuarterK5ClearEv+0x2>
 //1ec:	08 95       	ret
 
  	byte i;
	i=32;
	do {
		i--;
		fb[i]=0;
	} while (i!=0);
*/



/*

	// 15.2 s per 1000 000 calls
	// Sketch size 2484
//000001da <_ZN8QuarterK5ClearEv>:
// 1da:	e8 e0       	ldi	r30, 0x08	; 8
// 1dc:	f1 e0       	ldi	r31, 0x01	; 1
// 1de:	11 92       	st	Z+, r1
// 1e0:	81 e0       	ldi	r24, 0x01	; 1
// 1e2:	e8 32       	cpi	r30, 0x28	; 40
// 1e4:	f8 07       	cpc	r31, r24
// 1e6:	d9 f7       	brne	.-10     	; 0x1de <_ZN8QuarterK5ClearEv+0x4>
// 1e8:	08 95       	ret

/*
 	byte i;
	for (i=0; i<32; i++) {
		fb[i]=0;
	}

	// 11.5 s per 1000 000 calls
	// Sketch size 2484
	memset(fb,0,32);	
*/

/*
	// 5.2 s per 1000 000 calls
	// Sketch size 2598
//000001da <_ZN8QuarterK5ClearEv>:
 //1da:	10 92 08 01 	sts	0x0108, r1
 //1de:	10 92 09 01 	sts	0x0109, r1
 //1e2:	10 92 0a 01 	sts	0x010A, r1
 //1e6:	10 92 0b 01 	sts	0x010B, r1
 //1ea:	10 92 0c 01 	sts	0x010C, r1
 //1ee:	10 92 0d 01 	sts	0x010D, r1
 //1f2:	10 92 0e 01 	sts	0x010E, r1
 //1f6:	10 92 0f 01 	sts	0x010F, r1
 //1fa:	10 92 10 01 	sts	0x0110, r1
 //1fe:	10 92 11 01 	sts	0x0111, r1
 //202:	10 92 12 01 	sts	0x0112, r1
 //206:	10 92 13 01 	sts	0x0113, r1
 //20a:	10 92 14 01 	sts	0x0114, r1
 //20e:	10 92 15 01 	sts	0x0115, r1
 //212:	10 92 16 01 	sts	0x0116, r1
 //216:	10 92 17 01 	sts	0x0117, r1
 //21a:	10 92 18 01 	sts	0x0118, r1
 //21e:	10 92 19 01 	sts	0x0119, r1
 //222:	10 92 1a 01 	sts	0x011A, r1
 //226:	10 92 1b 01 	sts	0x011B, r1
 //22a:	10 92 1c 01 	sts	0x011C, r1
 //22e:	10 92 1d 01 	sts	0x011D, r1
 //232:	10 92 1e 01 	sts	0x011E, r1
 //236:	10 92 1f 01 	sts	0x011F, r1
 //23a:	10 92 20 01 	sts	0x0120, r1
 //23e:	10 92 21 01 	sts	0x0121, r1
 //242:	10 92 22 01 	sts	0x0122, r1
 //246:	10 92 23 01 	sts	0x0123, r1
 //24a:	10 92 24 01 	sts	0x0124, r1
 //24e:	10 92 25 01 	sts	0x0125, r1
 //252:	10 92 26 01 	sts	0x0126, r1
 //256:	10 92 27 01 	sts	0x0127, r1
 //25a:	08 95       	ret
 
 asm volatile(
    "sts fb+0, __zero_reg__"       "\n\t"
    "sts fb+1, __zero_reg__"       "\n\t"
    "sts fb+2, __zero_reg__"       "\n\t"
    "sts fb+3, __zero_reg__"       "\n\t"
    "sts fb+4, __zero_reg__"       "\n\t"
    "sts fb+5, __zero_reg__"       "\n\t"
    "sts fb+6, __zero_reg__"       "\n\t"
    "sts fb+7, __zero_reg__"       "\n\t"
    "sts fb+8, __zero_reg__"       "\n\t"
    "sts fb+9, __zero_reg__"       "\n\t"
    "sts fb+10, __zero_reg__"       "\n\t"
    "sts fb+11, __zero_reg__"       "\n\t"
    "sts fb+12, __zero_reg__"       "\n\t"
    "sts fb+13, __zero_reg__"       "\n\t"
    "sts fb+14, __zero_reg__"       "\n\t"
    "sts fb+15, __zero_reg__"       "\n\t"
    "sts fb+16, __zero_reg__"       "\n\t"
    "sts fb+17, __zero_reg__"       "\n\t"
    "sts fb+18, __zero_reg__"       "\n\t"
    "sts fb+19, __zero_reg__"       "\n\t"
    "sts fb+20, __zero_reg__"       "\n\t"
    "sts fb+21, __zero_reg__"       "\n\t"
    "sts fb+22, __zero_reg__"       "\n\t"
    "sts fb+23, __zero_reg__"       "\n\t"
    "sts fb+24, __zero_reg__"       "\n\t"
    "sts fb+25, __zero_reg__"       "\n\t"
    "sts fb+26, __zero_reg__"       "\n\t"
    "sts fb+27, __zero_reg__"       "\n\t"
    "sts fb+28, __zero_reg__"       "\n\t"
    "sts fb+29, __zero_reg__"       "\n\t"
    "sts fb+30, __zero_reg__"       "\n\t"
    "sts fb+31, __zero_reg__"       "\n\t"
	);
*/
		
}


	

void QuarterK::Set(void) {
	byte i;
	for (i=0; i<32; i++) {
		framebuffer[i]=0xFF;
	}
} 




void QuarterK::Plot(byte x, byte y, byte color) {
  byte address;
  byte mask;

  address=(y<<1)+((x>>3)&0x01);
	mask=1<<(x&0x07);

  if (color==1) {
		framebuffer[address]|=mask;
	} else {
		framebuffer[address]&=(~mask);
	}
}



void QuarterK::Line(byte x1, byte y1,
unsigned char x2, unsigned char y2,
unsigned char color )
{
  char deltax = abs(x2 - x1);        // The difference between the x's
  char deltay = abs(y2 - y1);        // The difference between the y's
  char x = x1;                       // Start x off at the first pixel
  char y = y1;                       // Start y off at the first pixel
  char xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

  if (x2 >= x1) {                // The x-values are increasing
    xinc1 = 1;
    xinc2 = 1;
  }  
  else {                          // The x-values are decreasing
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 // The y-values are increasing
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          // The y-values are decreasing
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         // There is at least one x-value for every y-value
  {
    xinc1 = 0;                  // Don't change the x when numerator >= denominator
    yinc2 = 0;                  // Don't change the y for every iteration
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         // There are more x-values than y-values
  }
  else                          // There is at least one y-value for every x-value
  {
    xinc2 = 0;                  // Don't change the x for every iteration
    yinc1 = 0;                  // Don't change the y when numerator >= denominator
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         // There are more y-values than x-values
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    Plot(x, y, color);             // Draw the current pixel
    num += numadd;              // Increase the numerator by the top of the fraction
    if (num >= den)             // Check if numerator >= denominator
    {
      num -= den;               // Calculate the new numerator value
      x += xinc1;               // Change the x as appropriate
      y += yinc1;               // Change the y as appropriate
    }
    x += xinc2;                 // Change the x as appropriate
    y += yinc2;                 // Change the y as appropriate
  }
}



/*

void loop() {
  int i;
  byte c;
  word  v;
  byte x;

  
  
  x=0;
    for (c=9; c<14; c++) {
    for (i=0; i<6; i++) {
      v=pgm_read_byte_near(font+c*6+i);
      v=v<<x;
      framebuffer[i*2+1]|=(v>>8);
      framebuffer[i*2]|=(v&0xFF);
    }
    x=x+1+pgm_read_byte_near(charwidth+c);
    }
  for (;;);

}


*/




prog_uchar font[] PROGMEM = {
    0B00000110, 0B00001001, 0B00001001, 0B00001111, 0B00001001, 0B00000000, //A
    0B00000110, 0B00001001, 0B00000111, 0B00001001, 0B00001111, 0B00000000, //B
    0B00000110, 0B00001001, 0B00000001, 0B00001001, 0B00000110, 0B00000000, //C
    0B00000110, 0B00001001, 0B00001001, 0B00001001, 0B00000111, 0B00000000, //D
    0B00001110, 0B00000001, 0B00000011, 0B00000001, 0B00001111, 0B00000000, //E
    0B00001111, 0B00000001, 0B00000111, 0B00000001, 0B00000001, 0B00000000, //F
    0B00001110, 0B00000001, 0B00001001, 0B00001001, 0B00001110, 0B00000000, //G
    0B00001001, 0B00001001, 0B00001111, 0B00001001, 0B00001001, 0B00000000, //H
    0B00000001, 0B00000001, 0B00000001, 0B00000001, 0B00000001, 0B00000000, //I
    0B00000010, 0B00000010, 0B00000010, 0B00000010, 0B00000001, 0B00000000, //J
    0B00001001, 0B00000101, 0B00000011, 0B00000101, 0B00001001, 0B00000000, //K
    0B00000001, 0B00000001, 0B00000001, 0B00000001, 0B00001110, 0B00000000, //L
    0B00001110, 0B00010101, 0B00010101, 0B00010101, 0B00010101, 0B00000000, //M
    0B00000110, 0B00001001, 0B00001001, 0B00001001, 0B00001001, 0B00000000, //N
    0B00000110, 0B00001001, 0B00001001, 0B00001001, 0B00000110, 0B00000000, //O
    0B00000110, 0B00001001, 0B00000111, 0B00000001, 0B00000001, 0B00000000, //P
    0B00000110, 0B00001001, 0B00001001, 0B00001001, 0B00001110, 0B00000000, //Q
    0B00000110, 0B00001001, 0B00001111, 0B00000101, 0B00001001, 0B00000000, //R
    0B00000110, 0B00000001, 0B00000110, 0B00001000, 0B00000110, 0B00000000, //S
    0B00011111, 0B00000100, 0B00000100, 0B00000100, 0B00000100, 0B00000000, //T
    0B00001001, 0B00001001, 0B00001001, 0B00001001, 0B00000110, 0B00000000, //U
    0B00001001, 0B00001001, 0B00001001, 0B00000101, 0B00000010, 0B00000000, //V
    0B00010001, 0B00010001, 0B00010101, 0B00010101, 0B00001010, 0B00000000, //W
    0B00010001, 0B00001010, 0B00000100, 0B00001010, 0B00010001, 0B00000000, //X
    0B00001001, 0B00001001, 0B00001110, 0B00001000, 0B00000110, 0B00000000, //Y
    0B00011111, 0B00001000, 0B00000100, 0B00000010, 0B00011111, 0B00000000, //Z
    0B00000110, 0B00001001, 0B00001001, 0B00001001, 0B00000110, 0B00000000, //0
    0B00000010, 0B00000011, 0B00000010, 0B00000010, 0B00000010, 0B00000000, //1
    0B00000111, 0B00001000, 0B00000110, 0B00000001, 0B00001111, 0B00000000, //2
    0B00000111, 0B00001000, 0B00000110, 0B00001000, 0B00000111, 0B00000000, //3
    0B00000100, 0B00000110, 0B00000101, 0B00001111, 0B00000100, 0B00000000, //4
    0B00001111, 0B00000001, 0B00000111, 0B00001000, 0B00000111, 0B00000000, //5
    0B00000110, 0B00000001, 0B00000111, 0B00001001, 0B00000110, 0B00000000, //6
    0B00001111, 0B00001000, 0B00000100, 0B00000010, 0B00000010, 0B00000000, //7
    0B00000110, 0B00001001, 0B00000110, 0B00001001, 0B00000110, 0B00000000, //8
    0B00000110, 0B00001001, 0B00001110, 0B00001000, 0B00000110, 0B00000000  //9
};

prog_uchar charwidth[] PROGMEM = {
    4,4,4,4,4,4,4,4,1,2,4,4,5,4,4,4,4,4,4,5,4,4,5,5,4,5,4,2,4,4,4,4,4,4,4,4
};




prog_uchar font3x4[] PROGMEM = {
    0x32,0x75,0x77,0x75, //A B
    0x36,0x51,0x51,0x36, //C D
    0x77,0x11,0x33,0x17, //E F
    0x56,0x51,0x75,0x56, //G H
    0x47,0x42,0x52,0x67, //I J
    0x11,0x15,0x13,0x75, //K L
    0x37,0x57,0x57,0x55, //M N
    0x32,0x55,0x35,0x12, //O P
    0x32,0x55,0x35,0x56, //Q R
    0x76,0x21,0x26,0x27, //S T
    0x55,0x55,0x55,0x27, //U V
    0x55,0x27,0x77,0x57, //W X
    0x75,0x45,0x22,0x72, //Y Z
    0x27,0x35,0x25,0x77, //0 1
    0x36,0x24,0x42,0x37, //2 3
    0x35,0x15,0x67,0x74, //4 5
    0x76,0x41,0x27,0x27, //6 7
    0x33,0x33,0x46,0x36  //8 9
};



void QuarterK::Draw3x4Character(char chr, byte x, byte y) {
  byte i;
  byte bitmap;
  word address;

  // Character can only be drawn at 4-bit boundries
  // so we only have 4 positions to draw it on.
  x&=3;


  if (chr==0x20) return;
  
  // Convert lowercase to upper case (ugly hack) 
  if (chr>0x5F) chr-=0x20;
  // Map A-Z as 0x00-0x19
  if (chr>0x40) {
    chr-=0x41;
  } else {
    chr-=0x16;
  }


  for (i=0; i<4; i++) {
    bitmap=pgm_read_byte_near(font3x4 + (chr/2)*4 + i);
	address=y*2 + i*2;
    if (((x&1)==0) && ((chr&1)==0)) { bitmap&=0x0f;  } 
    if (((x&1)==1) && ((chr&1)==0)) { bitmap<<=4;    } 
    if (((x&1)==0) && ((chr&1)==1)) { bitmap>>=4;    }
    if (((x&1)==1) && ((chr&1)==1)) { bitmap&=0xf0;  } 
    if (x>1) address++;
	framebuffer[address] |= bitmap;
  }
	
}




