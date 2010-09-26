/*
* QuarterK.cpp - Library to drive the QuarterK 16x16 Led Matrix shield
*
* v1.0 Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
*
* This software is licensed under the Creative Commons Attribution-
* ShareAlike 3.0 Unported License.
* http://creativecommons.org/licenses/by-sa/3.0/
*
*/

#include "WProgram.h"
#include "QuarterK.h"

#include <avr/pgmspace.h>


byte framebuffer[32];
volatile unsigned int tick;


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

    // Start timer1 used or refreshing the displays
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
    TCCR1A=0x00;
    TCCR1B=(_BV(WGM12) | _BV(CS11) | _BV(CS10)); //Divide by 64
    OCR1A=249;                    //Count to 250
    TIMSK1=_BV(OCIE1A);
    sei();                          // ensures that interrupts are globally enabled
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
    memset(framebuffer,0x00,32);
}




void QuarterK::Fill(void) {
    memset(framebuffer,0xFF,32);
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



void QuarterK::Line(byte x1, byte y1, unsigned char x2, unsigned char y2, unsigned char color ) {
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
    x=x>>2;
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





void QuarterK::Draw3x4Characters(char *str, byte x, byte y, byte yinc) {
    byte i;

    for (i=0; str[i]>0; i++) {
        Draw3x4Character(str[i], x, y);
        x+=4;
        if (x>12) {
            x=0;
            y+=yinc;
        }
    }
}


void QuarterK::Delay(unsigned int ms) {
    while (ms-->0) {
        unsigned int oldtick=tick;
        while(tick==oldtick);
    };
}




boolean QuarterK::ReadFireButton(byte controllerNo) {
    if (analogRead(controllerNo*3+2)>128) {
        return true;
    } else {
        return false;
    }
}


byte QuarterK::ReadDirButtons(byte controllerNo) {
    int v;
    
    v=analogRead(controllerNo*3+1);
    if (v<127) return 0;
    if (v<301) return BUTTON_RIGHT;
    if (v<429) return BUTTON_DOWN;
    if (v<767) return BUTTON_LEFT;
    return BUTTON_UP;
}


int QuarterK::ReadPaddle(byte controllerNo) {
    int v;

    v=analogRead(controllerNo*3);
    return v;
}
