/*
 * This sketch demonstrates the line drawing function of the QuarterK-library  
 * 
 * It draws a straight line diagonally across the display and moves the staring point
 * first across the top line from left to right then down along the right edge. This
 * gives the impression of havning the line rotate as a propeller. 
 *
 * Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 *
 */
 
#include <QuarterK.h>

QuarterK qk;

void setup() {
  // Clear display
  qk.Clear();
}


void loop() {
  byte x,y;

  // Move start coord along top edge
  y=0;
  for (x=0; x<16; x++) {
    // Draw a line from start coord diagaonally across 
    qk.Line(x,y, 15-x, 15-y, 1);
    // Wait for a while...
    delay(25);
    // Then erase the line
    qk.Line(x,y, 15-x, 15-y, 0);
  }

  // Move start coord along right edge
  x=15;
  for (y=1; y<15; y++) {
    // Draw a line from start coord diagaonally across 
    qk.Line(x,y, 15-x, 15-y, 1);
    // Wait for a while...
    delay(25);
    // Then erase the line
    qk.Line(x,y, 15-x, 15-y, 0);
  }
      
}

