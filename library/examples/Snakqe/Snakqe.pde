/*
 *    _____                _                
 *   / ____|              | |               
 *  | (___   _ __    __ _ | | __ __ _   ___ 
 *   \___ \ | '_ \  / _` || |/ // _` | / _ \
 *   ____) || | | || (_| ||   <| (_| ||  __/
 *  |_____/ |_| |_| \__,_||_|\_\\__, | \___|
 *                                 | |      
 *                                 |_|      
 * This is the classic game of Snake.  Gude the snake
 * to the flashing goal pixel while taking care of
 * not running in to yourself.  This implementation
 * got no walls so the snake is wrapping around to the
 * other side.
 * 
 * Requires a QK Gamepad
 *
 * v1.0 Created October 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 * This software is licensed under the Creative Commons Attribution-
 * ShareAlike 3.0 Unported License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * 
 */

#include <QuarterK.h>
#include <Arial8.h>

#define MAXLENGTH 200
#define MESSAGESPEED 40
#define SNAKESPEED  150

#define DIR_UP    -0x10
#define DIR_DOWN   0x10
#define DIR_LEFT  -0x01
#define DIR_RIGHT  0x01


void setup() {
}


//
//  Find a unoccupied location on the game board
//
byte NewGoal() {
  byte x,y;
  
  do {
    x=random(16);
    y=random(16);
  } while (qk.IsSet(x,y));
  return y*16+x;
}


//
// Flash the display for a second or two
//
void Explosion() {
  byte i,j;

  for (j=0; j<30; j++) {
    for (i=0; i<32; i++) {
      framebuffer[i]^=0xFF;
    }
    qk.Flip();
    qk.Delay(50);
  }

}



//
// Scroll two text messages back and forth across the display. 
// Return true if fire is pressed
//
boolean Scroll2Message(char *msg1, char *msg2) {
  int  shift1, shift2;
  char dir1, dir2;
  byte rnd;

  shift1=shift2=0;
  dir1=dir2=1;
  while (true) {
    rnd=random(255);
    if ((shift1>=0) && (!qk.DrawText(msg1, Arial_8, shift1, 0, 0))) {
      shift1+=dir1;
    } else {
      dir1=-dir1;
      shift1+=dir1;
    }
    if ((shift2>=0) && (!qk.DrawText(msg2, Arial_8, shift2, 0, 8))) {
      shift2+=dir2;
    } else {
      dir2=-dir2;
      shift2+=dir2;
    }
    qk.Flip();
    qk.Clear();
    qk.Delay(MESSAGESPEED);
    if (qk.ReadFireButton()) return true;   
  }
  return false;
}



void loop() {
  byte snake[MAXLENGTH];  // x-pos in low nybble, y-pos in high nybble
  byte snakelen;
  char snakedir;
  byte goal;
  boolean ingame;
  byte  buttons;
  byte speed;
  int i;
  boolean click;
  int score;
  int tmp;
  int steps;
  char tmps[30];

  while (!Scroll2Message("    ~~ Snaqke  ~~","    Press fire to start"));  

  qk.Clear();
  qk.Flip();
  snakelen=2;
  snake[0]=0x73;
  snake[1]=0x72;
  snakedir=DIR_RIGHT;
  speed=SNAKESPEED;
  goal=NewGoal();
  ingame=true;
  steps=0;
  score=0;

  while (ingame) {
    steps++;
    qk.Clear();

    // Flash the goal pixel
    if (steps&0x1) {
      qk.Plot(goal&0x0F, goal>>4);
    }

    // Plot all the snake pixels
    for (i=0; i<snakelen; i++) {
      qk.Plot(snake[i]&0x0F, snake[i]>>4);
    }
    qk.Flip();
   
    // Check if the snake reached the goal pixel.
    if (snake[0]==goal) {
      // Choose new goal pixel
      goal=NewGoal();
      qk.Plot(goal&0x0F, goal>>4);
      qk.Flip();
      // Make snake one unit longer
      if (snakelen<MAXLENGTH-1) {
        snake[snakelen]=snake[snakelen-1];
        snakelen++;
      }
      // Increase the score
      tmp=snakelen*10-steps;
      if (tmp>0) score+=tmp;
      steps=0;
    }

    // Read buttons 10 times during the step-delay. If read only once during the full delay
    // a short press might be missed
    for (i=0; i<10; i++) {
      qk.Delay(speed/10);
      buttons=qk.ReadDirButtons();    
      if ((buttons&BUTTON_UP)    && (snakedir!=DIR_DOWN))  snakedir=DIR_UP;
      if ((buttons&BUTTON_DOWN)  && (snakedir!=DIR_UP))    snakedir=DIR_DOWN;
      if ((buttons&BUTTON_LEFT)  && (snakedir!=DIR_RIGHT)) snakedir=DIR_LEFT;
      if ((buttons&BUTTON_RIGHT) && (snakedir!=DIR_LEFT))  snakedir=DIR_RIGHT;
    }
    
    // Shift snake coords towards snake end 
    for (i=snakelen-1; i>0; i--) {
      snake[i]=snake[i-1];
    }
    
    // Move head in the desired direction taking care of wrapping at
    // left & right edges
    if (((snake[0]&0x0F)==0) && (snakedir==DIR_LEFT)) {
      snake[0]+=15;
    } else if (((snake[0]&0x0F)==15) && (snakedir==DIR_RIGHT)) {
      snake[0]-=15;
    } else {
      snake[0]+=snakedir;
    }

    // Check if snake collided with itself
    for (i=1; i<snakelen; i++) {
      if (snake[0]==snake[i]) {
        Explosion();
        ingame=false;
        break;
      }
    }

  }

  // Display end of game message
  sprintf(tmps,"    Score: %d  Length: %d", score, snakelen);
  while (!Scroll2Message("    Game over",tmps));  
}

