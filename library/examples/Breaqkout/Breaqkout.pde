/*
 * ______                      _               _   
 * | ___ \                    | |             | |  
 * | |_/ /_ __ ___  __ _  __ _| | _____  _   _| |_ 
 * | ___ \ '__/ _ \/ _` |/ _` | |/ / _ \| | | | __|
 * | |_/ / | |  __/ (_| | (_| |   < (_) | |_| | |_ 
 * \____/|_|  \___|\__,_|\__, |_|\_\___/ \__,_|\__|
 *                          | |                    
 *                          |_|                    
 *
 * A simpleminded implementation of the classic "Breakout"
 * arcade game to be used with the QuarterK shield.
 * 
 * Requires a potemtiometer connected to Analog input 0
 * and a button on Analog input 1. Analog 1 should have
 * a pulldown resistor and the be taken to VCC when the
 * button is pressed.  The QK Gamepad would be ideal for this. :-)
 *
 * v1.0 Created September 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 * This software is licensed under the Creative Commons Attribution-
 * ShareAlike 3.0 Unported License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * 
 */

#include <QuarterK.h>
#include <ArialBold16.h>

#define SPEAKER  8

#define  COLS  4
#define  ROWS  4
#define  BRICKWIDTH  2
#define  BRICKSTEP   4

#define  PADDLEWIDTH  3

#define UP      -1
#define DOWN     1
//#define STRAIGHT 0
//#define LEFT     -1
//#define RIGHT    1

#define TOPEDGE 0
#define BOTTOMEDGE 15
#define LEFTEDGE 0
#define RIGHTEDGE 15

#define LEVEL1SPEED  80       // Speed of ball for firsr level
#define SPEEDINCREASE  7    // Speed increase for each level
#define MAXSPEED    30      // Maximum speed for the game

#define LIVES        5      // Nr of lives

#define MODE_WAITFORSTART  0
#define MODE_PLAYING       1
#define MODE_GAMEOVER      2

#define MESSAGESPEED      40


byte bricks[ROWS][COLS];
byte lives;
int  score;
byte level;
char ballX;
char ballY;
char ballYdir;
char ballXangle;



void setup() {
}


//
// Set ball to starting position
//
void ResetBall() {
  // Put tbe ball on the bottom
  ballX=7;
  ballY=14;
  ballYdir=UP;
  ballXangle=-30+random(0,60);;
}




//
//
//
void NextLevel() {
  level++;

  //  Turn on all bricks in the brick array
  for (byte r=0; r<ROWS; r++) {
    for (byte c=0; c<COLS; c++) {
      bricks[r][c]=1;
    }
  }
  
  ResetBall();
}



//
//
//
void NewGame() {
  lives=LIVES;  
  score=0;
  level=0;  // Incremented to 1 by NewLevel()
  NextLevel();
}



//
// Redraw the current status of all bricks on the screen
//
void DrawBrickfield() {
  for (byte r=0; r<ROWS; r++) {
    for (byte c=0; c<COLS; c++) {
      if (bricks[r][c]==1) {
        qk.Line(c*BRICKSTEP, r*2, c*BRICKSTEP+BRICKWIDTH, r*2, 1);
      } else {
        qk.Line(c*BRICKSTEP, r*2, c*BRICKSTEP+BRICKWIDTH, r*2, 0);
      }
    }
  }
}



//
// Count the number of bricks left
//
byte CountBricks() {
  byte brickCnt=0;
  
  for (byte r=0; r<ROWS; r++) {
    for (byte c=0; c<COLS; c++) {
      if (bricks[r][c]==1) brickCnt++;
    }
  }
  return brickCnt;
}



//
// Scroll intro text across the display. If abortable is set 
// then return true if fire is pressed
//
boolean ScrollMessage(char *msg, boolean abortable=true) {
  int      shift;

  shift=0;
  while (!qk.DrawText(msg, ArialBold_16, shift++)) {
    qk.Flip();
    qk.Clear();
    qk.Delay(MESSAGESPEED);
    if (abortable && qk.ReadFireButton()) return true;   
  }
  return false;
}




void WaitForNextLevelPress() {
  char  tmps[20];
  char  cnt=0;
  int   paddle;
  
  ResetBall();

  // Loop here until the fire button is pressed
  while (!qk.ReadFireButton()) {
    qk.Delay(50);
    cnt++;

    // Get new paddle position
    paddle=qk.ReadPaddle()/76;
    if (paddle>RIGHTEDGE-(PADDLEWIDTH-1)) paddle=RIGHTEDGE-(PADDLEWIDTH-1);

    // Redraw entire screen
    qk.Clear();
    qk.Line(paddle/1, BOTTOMEDGE, paddle+(PADDLEWIDTH-1), BOTTOMEDGE, 1);

    // Flash the ball
    if (cnt&0x4) {
      qk.Plot(ballX, 14, 1);
    }

    if ((cnt&0x60)==0x00) {
      qk.Draw3x4Characters("PresFire", 0, 0);
    }

    if ((cnt&0x60)==0x20) {
      sprintf(tmps,"LIVE %02d ", lives);
      qk.Draw3x4Characters(tmps, 0 , 0, 7);
    }

    if ((cnt&0x60)==0x40) {
      sprintf(tmps,"SCOR%04d", score);
      qk.Draw3x4Characters(tmps, 0, 0, 7);
    }

    if ((cnt&0x60)==0x60) {
      DrawBrickfield();
    }

    qk.Flip();
  }

  qk.Clear();
  DrawBrickfield();
  qk.Flip();
}





void loop() {
  char ballXold;
  char ballYold;
  int  xcounter;
  int  paddle;
  int buttons;
  char tmps[20];
  boolean gameover;
  
  ScrollMessage("    Breaqkout    ", false);
  
  NewGame();
  WaitForNextLevelPress();
  xcounter=0;
  gameover=false;

  while (!gameover) {
    qk.Flip();
    qk.Delay(max(MAXSPEED, LEVEL1SPEED-SPEEDINCREASE*level));

    // Erase old paddle, calculate new position and re-draw it
    qk.Line(paddle/1, BOTTOMEDGE, paddle+(PADDLEWIDTH-1), BOTTOMEDGE, 0);
    paddle=qk.ReadPaddle()/76;
    if (paddle>RIGHTEDGE-(PADDLEWIDTH-1)) paddle=RIGHTEDGE-(PADDLEWIDTH-1);
    qk.Line(paddle/1, BOTTOMEDGE, paddle+(PADDLEWIDTH-1), BOTTOMEDGE, 1);
     
    // Move ball up/down one pixel for each turn
    ballY+=ballYdir;

    // Move ball horizontally according to the angle. 0=straight up
    // more negative=more left, more positive=more right.    
    xcounter+=abs(ballXangle);
    if (xcounter>=64) {
      xcounter-=64;
      if (ballXangle>0) ballX++;
      if (ballXangle<0) ballX--;
    }

    // Erase the old ball from the screen and draw the ball
    // in its new position
    qk.Plot(ballXold, ballYold, 0);
    qk.Plot(ballX, ballY, 1);
    ballXold=ballX;
    ballYold=ballY;

    //
    // Check for collision with a brick 
    //
    // Bricks are only on even lines
    if ((ballY&1)==0) { 
      char brickrow=ballY/2;
      // Are ball high enough to possibly have reached any bricks?
      if (brickrow<ROWS) {  
        // Are ball on a column that got brick coverage?
        if ((ballX%BRICKSTEP)<=BRICKWIDTH) {
          char brickcol=ballX/BRICKSTEP;
          // Check the brick array if there's a brick in this position
          if (bricks[brickrow][brickcol]==1) {
            // Yes!  Remove brick from array and redraw bricks & ball
            score+=5;
            bricks[brickrow][brickcol]=0;
            DrawBrickfield();
            qk.Plot(ballX, ballY, 1);
            tone(SPEAKER, 1200, 100);
            // Don't reverse direction if we're on the top-line, the 
            // tests further down will take care of top-line reversal.
            // Also don't revese if the ball is already going down
            if ((ballY!=TOPEDGE) && (ballYdir==UP)) { 
              ballYdir=-ballYdir;
            }
          }
        }
      }
    } 


    // Bounce ball if we reached the top line
    if (ballY==TOPEDGE) {
      ballYdir=-ballYdir;
      if (score>0) score--;
    }

    // Bounce ball if hit paddle at bottom line
    if (ballY==BOTTOMEDGE) {
      // Paddle hit?
      if (ballX>=paddle && ballX<paddle+PADDLEWIDTH) {
        if (ballX==paddle) ballXangle=-20+random(0,10);
        if (ballX==paddle+1) {
           if (random(0,10)>4) ballXangle=0;
           else if (random(0,10)>5) ballXangle=-4;
           else ballXangle=4;

        }
        if (ballX==paddle+2) ballXangle=20+random(0,10);
        tone(SPEAKER, 300, 100);
        ballYdir=-ballYdir;
      } else {
        // Ball missed paddle. Wait for press of fire button again unless game is over
        for (int i=1000; i>50; i--) tone(SPEAKER,i, 10);
        lives--;
        if (lives==0) {
          gameover=true;
        } else {
          WaitForNextLevelPress();
          ballXold=ballX;
          ballYold=ballY;
        }
        continue;
      }
    }

    // Bounce ball at left & right edges
    // Exit with same angle
    if (ballX==LEFTEDGE || ballX==RIGHTEDGE) {
      ballXangle=-ballXangle;
      // Make sure that next y-step generates a x-step
      // or we'll might end up locked to the edge
      xcounter+=64;
    }

    if (CountBricks()==0) {
      sprintf(tmps, "    Level %d complete", level);   
      ScrollMessage(tmps);
      NextLevel();
      WaitForNextLevelPress();
      ballXold=ballX;
      ballYold=ballY;
      continue;
    }


  } 
  
  // Scroll end of game messsge util user presses fire.
  sprintf(tmps, "    Game over. Score %d at level %d", score, level);   
  while (!ScrollMessage(tmps));
}

