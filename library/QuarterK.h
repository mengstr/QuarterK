/*
 * QuarterK.h - Library to drive the QuarterK 16x16 Led Matrix shield
 *
 * v1.0 Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
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

extern byte framebuffer[32];

class QuarterK {
	public:
		QuarterK(void);
		void Delay(unsigned int ms);
		void Clear(void);
		void Set(void);
		void Plot(byte x, byte y, byte color);
		void Line(byte x1, byte y1, byte x2, byte y2, byte color);
		void ShiftLeft(void);
		void ShiftRight(void);
		void ShiftUp(void);
		void ShiftDown(void);
		void Draw3x4Character(char chr, byte x, byte y);
	private:
		void InitializeTimer1(void);
};
	
#endif

