/*
 * This sketch demonstrates the text drawing functions of the QuarterK-library  
 * 
 *
 * Created August 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 *
 */
 
#include <QuarterK.h>
#include <ArialBold16.h>
#include <Arial16.h>
#include <Arial14.h>
//#include <Arial12.h>
#include <Arial11.h>
//#include <Arial10.h>
//#include <Arial9.h>
#include <Arial8.h>

void setup() {
  randomSeed(analogRead(5)); // Seed the random generator
}

void loop(void) {
  char     msg[]="    Hello world! 123abc";
  int      shift;
  uint8_t  *pFont;
  int      fontNo;
  boolean  done;
  char     tmps[5];
  int      i;
  
  // Draw a "Helo" and some random numbers using the ultrasmall
  // 3x4 font
  for (i=0; i<30; i++) {
    qk.Clear();
    qk.Draw3x4Characters("Helo",0,0);
    sprintf(tmps,"%04d", random(0,10000));
    qk.Draw3x4Characters(tmps,0,6);
    qk.Flip();
    qk.Delay(150);    
  }


  // Scroll a short message using some sample fonts
  for (fontNo=0; fontNo<5; fontNo++) {
    switch (fontNo) {
      case 0: pFont=Arial_8;
              break;
      case 1: pFont=Arial_11;
              break;
      case 2: pFont=Arial_14;
              break;
      case 3: pFont=Arial_16;
              break;
      case 4: pFont=ArialBold_16;
              break;
      default:
              pFont=Arial_8;
              break;
    }

    shift=0;
    do {
      qk.Clear();
      done=qk.DrawText(msg, pFont, shift);
      qk.Flip();
      qk.Delay(40);
      shift++;
    } while (!done);

  }
}  



