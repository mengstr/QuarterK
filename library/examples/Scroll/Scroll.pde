/*
 * This sketch demonstrates the scrolling functions of the QuarterK-library  
 * 
 * It draws a Space Invader ailen symbol in th eupper left corner of the display and
 * then scrolls it aqound the display by shifting it 8 steps to the left, down, right and
 * then back up again. 
 *
 * Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 */
#include <QuarterK.h>

QuarterK qk;

void setup() {
  // Clear framebuffer
  qk.Clear();
  // Draw a 8-by-8 pixel space invader alien in the to left quadrant
  framebuffer[0] =0x18;
  framebuffer[2] =0x3c;
  framebuffer[4] =0x7e;
  framebuffer[6] =0xdb;
  framebuffer[8] =0xff;
  framebuffer[10]=0x24;
  framebuffer[12]=0x42;
  framebuffer[14]=0x24;
}



void loop() {
  byte i;
  byte speed=50;

  // Move ailen 8 steps to the right
  for (i=0; i<8; i++) {
    qk.ShiftRight();
    qk.Delay(speed);
  }  

  // Move ailen 8 lines down
  for (i=0; i<8; i++) {
    qk.ShiftDown();
    qk.Delay(speed);
  }  
  
  // Move ailen 8 steps to the left
  for (i=0; i<8; i++) {
    qk.ShiftLeft();
    qk.Delay(speed);
  }  

  // Move ailen 8 lines up
  for (i=0; i<8; i++) {
    qk.ShiftUp();
    qk.Delay(speed);
  }  

}

