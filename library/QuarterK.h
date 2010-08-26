/*
 *  QuarterK.h - Library to drive the QuarterK 16x16 Led Matrix shield
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

