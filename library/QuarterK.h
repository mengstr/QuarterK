/*
 * QuarterK.h - Library to drive the QuarterK 16x16 Led Matrix shield
 *
 * v1.0 Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
 * v1.1 Mats Engstrom - Added ReadXXXX-functions and comments
 *
 * This software is licensed under the Creative Commons Attribution-
 * ShareAlike 3.0 Unported License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

#ifndef QuarterK_h
#define QuarterK_h

#include "WProgram.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define SCK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define SS_PIN    10

#define BUTTON_UP		1
#define BUTTON_DOWN		2
#define BUTTON_LEFT 	4
#define	BUTTON_RIGHT	8

extern byte framebuffer[32];

// The framebuffer is a binary representation of the leds on the display.
// There are 16x16 = 256 leds on the display the framebuffer have 32 bytes
// with 8 bits each giving 32x8 = 256 bits. One bit for each led/pixel.
//
// The first byte of the framebuffer is controlling the eight pixels in the
// top left corner of the display(Row 0 Columns 0-7). This is represented by
// the "a A A A A A A A" on the diagram below.
//
// The lowest order bit, bit 0, controls the first pixel (the small "a" in
// the diagram). The second bit is the second pixel and so on.
//
// The second byte of the frambuffer is controlling Row 0 Columns 8-15. This
// is repeated all the way down to the last byte the framebuffer that controls
// the pixels at Row 15 Columns 8-15.
//
// Example 1: Light the top left and the bottom right led.
//
// The lowest order bit of the first byte and the highest order bit of the last
// byte of the framebuffer array needs to be set.
// 		framebuffer[0]=0b00000001     // Top left corner
// 		farmebuffer[31]=0b10000000;   // Bottom right corner
//
//
// Example 2: Display the following on the display. (X=Led on=1 .=Led off=0)
//
//  First 8 bits	Next 8 bits
//  =============== ===============
//	X X X . X X X . X . . . . . . .
//	X . . . X . X . X . . . . . . .
//	X X X . X X X . X . . . . . . .
//	. . X . X X . . X . . . . . . .
//	X X X . X . X . X X X . . . . .
//
// On the first line we cn see that the bits 0,1,2,4,5,6 should be set in the first byte
// and bits 0 should be set in the second byte. This gives:
// 		framebuffer[0]=0b01110111;
//		framebuffer[1]=0b00000001;
//
// Instead of using binary numbers (denoted by starting the number with 0b) it can also
// be given in a shorter hexadecimal form by starting the number with 0x. The two lines
// above should then become:
// 		framebuffer[0]=0x77;	//0b0111 0111;
//		framebuffer[1]=0x01; 	//0b0000 0001;
//
// The entrire exaxmple 2 in binary would look like this:
// 		framebuffer[0]=0b01110111;	// First line, part 1
//		framebuffer[1]=0b00000001;  // First line, part 2
//
// 		framebuffer[0]=0b01010001;	// Second line, part 1
//		framebuffer[1]=0b00000001;  // Second line, part 2
//
// 		framebuffer[0]=0b01110111;	// Third line, part 1
//		framebuffer[1]=0b00000001;  // Third line, part 2
//
// 		framebuffer[0]=0b00110100;	// Fourth line, part 1
//		framebuffer[1]=0b00000001;  // Fourth line, part 2
//
// 		framebuffer[0]=0b01010111;	// Fifth line, part 1
//		framebuffer[1]=0b00000111;  // Fifth line, part 2
//
//
//
//		  	          Column
//			                    1 1 1 1 1 1
//  		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
// Row  0	a A A A A A A A b B B B B B B B
// Row  1	c C C C C C C C d D D D D D D D
// Row  2
// Row  3
// Row  4
// Row  5
// Row  6
// Row  7
// Row  8
// Row  9
// Row 10
// Row 11
// Row 12
// Row 13
// Row 14	w W W W W W W W x X X X X X X X
// Row 15	y Y Y Y Y Y Y Y z Z Z Z Z Z Z Z
//
// 		aAAAAAAA is framebuffer[0];
// 		bBBBBBBB is framebuffer[1];
// 		zZZZZZZZ is framebuffer[31];
//




class QuarterK {

    public:
        QuarterK(void);

        // Turn off (Clear) or on (Fill) all pixels on the display
        void Clear(void);
        void Fill(void);

        // Set an individual pixel to desired state. 1=on (default) 0=off.
        // Top left corner is 0,0, bottom right is 15,15.
        void Plot(byte x, byte y, byte color=1);

        // Draw a line from x1,y1 to x2,y2. Top left corner is 0,0, bottom
        // right corner is 15,15. The color of the line defaults to on (1)
        // off is 0.
        void Line(byte x1, byte y1, byte x2, byte y2, byte color=1);

        // Shift the entrire display one row or column th the left/right/up/down
        // The pixels falling off the edge is lost and a new unlit row/column is
        // added at the opposite edge
        void ShiftLeft(void);
        void ShiftRight(void);
        void ShiftUp(void);
        void ShiftDown(void);

        // Draws a single character from a limited character set (currently A-Z and 0-9 only)
        // on the specified position on the display. Currently the x position is limited to
        // the values 0, 4, 8, 12. The y position can range from 0 to 12.
        void Draw3x4Character(char chr, byte x=0, byte y=0);

        // Draws a string of 3x4 characters. The string is wrapping and continuing
        // 5 (default, can be adjusted with the yinc parameter) rows down.
        void Draw3x4Characters(char *str, byte x=0, byte y=0, byte yinc=5);

        // Get the state of the fire-button on the game controller. Returns
        // true if it is pressed. The controllerNo defaults to the first controller
        // but can also be set to 1 to read the scond controller.
        boolean ReadFireButton(byte controllerNo=0);

        // Get the state of the Direction buttons. Returns on of the BUTTON_xxx
        // constants depending on which button is pressed. The current implementation
        // of the game controller hardware only allows for one button to be pressed at
        // a time. The controllerNo defaults to the first controller  but can also be
        // set to 1 to read the scond controller.
        byte ReadDirButtons(byte controllerNo=0);

        // Get the position of the potentiometer on the game controller. The position is
        // a number ranging from 0 to 1023. The controllerNo defaults to the first
        // controller  but can also be set to 1 to read the scond controller.
        int	ReadPaddle(byte controllerNo=0);

        // Delays for the specified number of milliseconds. This function must be used
        // instead of the native Arduino delay() function since the Arduino function is
        // making use of the timer used by the QuarterK library
        void Delay(unsigned int ms);

    private:
        void InitializeTimer1(void);
};

#endif

