/*
 *   _____          _        _     
 *  |_   _|        | |      (_)    
 *    | | ___  __ _| |_ _ __ _ ___ 
 *    | |/ _ \/ _` | __| '__| / __|
 *    | |  __/ (_| | |_| |  | \__ \
 *    \_/\___|\__, |\__|_|  |_|___/
 *               | |               
 *               |_|               
 *  
 *
 * v1.0 Created September 2010 by Mats Engstrom <mats@smallroomlabs.com>
 *
 * This software is licensed under the Creative Commons Attribution-
 * ShareAlike 3.0 Unported License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * 
 */


/*

   0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
00 X · · X X O X · · · · X · X X ·
01 X · · X X X X · · · · X · X X ·
02 X · · · · · · · · · · X · X X ·
03 X · · · · · · · · · · X · X X ·
04 X · · · · · · · · · · X · · · ·
05 X · · · · · · · · · · X · X X X
06 X · · · · · · · · · · X · X · X
07 X · · · · · · · · · · X · X X X
08 X · · · · · · · · · · X · X · X
09 X · · · · · · · · · · X · X X X
10 X · · · · · · · · · · X · · · ·
11 X · · · · · · · · · · X · X X X
12 X · · · · · · · · · · X · X · X
13 X · · · · · · · · · · X · X X X
14 X · · · · · · · · · · X · X · X
15 X · · · · · · · · · · X · X X X

Tetris info at 
http://www.colinfahey.com/tetris/tetris_en.html

actualLevel      iterationDelay [seconds]
                (rounded to nearest 0.05)
============    =========================
     1                 0.50
     2                 0.45
     3                 0.40
     4                 0.35
     5                 0.30
     6                 0.25
     7                 0.20
     8                 0.15
     9                 0.10
    10                 0.05
iterationDelay = ((11 - actualLevel) * 0.05);  // [seconds]    
    
actualLevel   free-fall   instant-drop
                points       points
===========   =========   ============
     1             6           24
     2             9           27
     3            12           30
     4            15           33
     5            18           36
     6            21           39
     7            24           42
     8            27           45
     9            30           48
    10            33           51    
pointAward = ( (21 + (3 * actualLevel)) - freeFallIterations );

*/

#include <QuarterK.h>
#include <ArialBold16.h>

#define MESSAGESPEED  40
#define DROPSPEED      5
#define KOROBEINIKISPEED  50

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int korobeiniki[] PROGMEM = {
    NOTE_E6, 4,
    NOTE_B5, 2,
    NOTE_C6, 2,
    NOTE_D6, 2,
    NOTE_E6, 1,
    NOTE_D6, 1,
    NOTE_C6, 2,
    NOTE_B5, 2,
    NOTE_A5, 4,
    NOTE_A5, 2,
    NOTE_C6, 2,
    NOTE_E6, 4,
    NOTE_D6, 2,
    NOTE_C6, 2,
    NOTE_B5, 6,
    NOTE_C6, 2,
    NOTE_D6, 4,
    NOTE_E6, 4,
    NOTE_C6, 4,
    NOTE_A5, 4,
    NOTE_A5, 8,
    0, 0
};


char     tmps[40];


void PlayTune(int *tune, int speed) {
  int freq;
  int duration;
  
  do { 
    freq=pgm_read_word_near(tune++);
    duration=pgm_read_word_near(tune++);
    noTone(8);
    if (freq>0) {
      tone(8, freq, duration*speed);
      delay(duration*speed*1.3);
    }
  } while (freq>0);
}



/*
  3x5 Font
  XXX  .X.  XXX  XXX  X.X  XXX  X..  XXX  XXX  XXX
  X.X  XX.  ..X  ..X  X.X  X..  X..  ..X  X.X  X.X
  X.X  .X.  XXX  XXX  XXX  XXX  XXX  .X.  XXX  XXX
  X.X  .X.  X..  ..X  ..X  ..X  X.X  .X.  X.X  ..X
  XXX  XXX  XXX  XXX  ..X  XXX  XXX  .X.  XXX  ..X
*/


uint16_t font3x5[] PROGMEM = {
  0b0111111000111111,   //0
  0b0100001111110010,   //1
  0b0101111010111101,   //2
  0b0111111010110101,   //3
  0b0111110010000111,   //4
  0b0111011010110111,   //5
  0b0111001010011111,   //6
  0b0000111110100001,   //7
  0b0111111010111111,   //8
  0b0111110010100111    //9
};



uint16_t pieces[] PROGMEM = {
    // O  · · · ·
    //    · X O ·
    //    · X X ·
    //    · · · ·
    0b0000011001100000,
    0b0000011001100000,
    0b0000011001100000,
    0b0000011001100000,

    
    // I  · · · ·    · · X ·
    //    X X O X    · · O ·
    //    · · · ·    · · X ·
    //    · · · ·    · · X ·
    0b0000000011110000,
    0b0100010001000100,
    0b0000000011110000,
    0b0100010001000100,
    
    // S  · · · ·    · · X ·
    //    · · O X    · · O X
    //    · X X ·    · · · X
    //    · · · ·    · · · ·
    0b0000011011000000,
    0b0000100011000100,
    0b0000011011000000,
    0b0000100011000100,
    
    // Z  · · · ·    · · · X
    //    · X O ·    · · O X
    //    · · X X    · · X ·
    //    · · · ·    · · · · 
    0b0000110001100000,
    0b0000010011001000,
    0b0000110001100000,
    0b0000010011001000,
    
    
    // L  · · · ·    · · X ·    · · · X    · X X ·
    //    · X O X    · · O ·    · X O X    · · O ·
    //    · X · ·    · · X X    · · · ·    · · X ·
    //    · · · ·    · · · ·    · · · ·    · · · ·
    0b0000001011100000,
    0b0000110001000100,
    0b0000000011101000,
    0b0000010001000110,
    
    // J  · · · ·    · · X X    · X · ·    · · X ·
    //    · X O X    · · O ·    · X O X    · · O ·
    //    · · · X    · · X ·    · · · ·    · X X ·
    //    · · · ·    · · · ·    · · · ·    · · · ·
    0b0000100011100000,
    0b0000010001001100,
    0b0000000011100010,
    0b0000011001000100,
    
    // T  · · · ·    · · X ·    · · X ·    · · X ·
    //    · X O X    · · O X    · X O X    · X O ·
    //    · · X ·    · · X ·    · · · ·    · · X ·
    //    · · · ·    · · · ·    · · · ·    · · · ·
    0b0000010011100000,
    0b0000010011000100,
    0b0000000011100100,
    0b0000010001100100
};





void Draw3x5Digit(byte no, char x, char y) {
  uint16_t  v;
  uint16_t  mask;
  
  char xx;
  char yy;

  mask=1;
  for (xx=0; xx<3; xx++) {
    for (yy=0; yy<5; yy++) {
      v=pgm_read_word_near(font3x5+no);
      if (v & mask) {
        qk.Plot(x+xx, y+yy, 1);
      } else {
        qk.Plot(x+xx, y+yy, 0);
      }
      mask<<=1;
    }
  }
}


uint8_t introScreen[] PROGMEM = {
    0b00111101,	0b11101100,
    0b10000001,	0b10001101,
    0b11011011,	0b00100000,
    0b00011000,	0b11101110,
    0b01000011,	0b00000010,
    0b01110001,	0b10011000,
    0b00000101,	0b11001101,
    0b01101100,	0b10000001,
    0b01101001,	0b00001001,
    0b00000001,	0b01101101,
    0b01001101,	0b11001000,
    0b01101101,	0b00000011,
    0b00100000,	0b01111011,
    0b00001010,	0b00000000,
    0b10011011,	0b11001001,
    0b11001001,	0b11011100
};


void IntroScreen () {
  qk.Clear();
  for (byte i=0; i<32; i++) {
    framebuffer[i]=pgm_read_byte_near(introScreen+i);;
  }  
  qk.Flip();
}



void PreviewPiece(byte n) {
  uint16_t  v;
  uint16_t  mask;
  char x,y;
  
  v=pgm_read_word_near(pieces+n*4);
  mask=1;
  
  for (x=12; x<16; x++) {
    for (y=3; y>-1; y--) {
      if (v&mask) {
        qk.Plot(x,y,1);
      } else {
        qk.Plot(x,y,0);
      }
      mask<<=1;
    }
  }
}



//boolean SpawnPiece(byte piece) {
//  boolean overlap;
//  uint16_t  v;
//  uint16_t  mask;
//  byte      x,y;
//  
//  v=pgm_read_word_near(pieces+piece*4);
//  mask=1<<4;  // Set mask to skip first row
//  
//  overlap=false;
//  for (y=0; y<2; y++) {  // Only handle two middle rows
//    for (x=4; x<8; x++) {
//      if (v&mask) {
//        if (qk.IsSet(x,y)) overlap=true;
//        qk.Plot(x,y);
//      }
//      mask<<=1;
//    }
//  }
//  return overlap;
//}



void PlotThePiece(char piece, char pieceX, char pieceY, char pieceR, char color) {
  uint16_t  v;
  uint16_t  mask;
  char      x,y;
  
  v=pgm_read_word_near(pieces+piece*4+pieceR);
  mask=1;
  
  for (y=pieceY; y<pieceY+4; y++) {  
    for (x=pieceX+1; x<pieceX+4+1; x++) {
      if ((y>=0) && (y<=15) && (x>=1) && (x<=10)) {
        if (v&mask) {
          qk.Plot(x,y,color);
        }
      }
      mask<<=1;
    }
  }
  
}


void DrawPiece(char piece, char pieceX, char pieceY, char pieceR) {
  PlotThePiece(piece, pieceX, pieceY, pieceR, 1);
}

void UndrawPiece(char piece, char pieceX, char pieceY, char pieceR) {
  PlotThePiece(piece, pieceX, pieceY, pieceR, 0);
}

boolean IsPieceCollision(char piece, char pieceX, char pieceY, char pieceR) {
  uint16_t  v;
  uint16_t  mask;
  char      x,y;
  boolean overlap;
  
  v=pgm_read_word_near(pieces+piece*4+pieceR);
  mask=1;
  overlap=false;
  
  for (y=pieceY; y<pieceY+4; y++) {  
    for (x=pieceX+1; x<pieceX+4+1; x++) {
      if (v&mask) {
        if ((y>=0) && (y<=15) && (x>=1) && (x<=10)) {
          if (qk.IsSet(x,y)) {
            overlap=true;
          }
        } else {
          overlap=true;
        }
      }
      mask<<=1;
    }
  }


//  if (overlap) {noTone(8); tone(8, 2000, 500); qk.Delay(600);}
  return overlap;  
}



byte CompletedLines() {
  byte x,y;
  byte yy;
  byte cnt;
  byte rowcnt;
  
  rowcnt=0;
  for (y=0; y<16; y++) {
    cnt=0;
    for (x=1; x<11; x++) {
      if (qk.IsSet(x,y)) cnt++;
    }

    if (cnt==10) {  // The line is complete
      //Shift all lines above this down one step 
      rowcnt++;
      for (yy=y; yy>0; yy--) {
        for (x=1; x<11; x++) {
          if (qk.IsSet(x,yy-1)) {
            qk.Plot(x,yy,1);
          } else {
            qk.Plot(x,yy,0);
          }
        }
      }
      // Clear toprow
      for (x=1; x<11; x++) {
        qk.Plot(x,0,0);
      }
    }

  }

  // Refresh screen if any lines was removed
  if (rowcnt>0) qk.Flip(); 
  return rowcnt;
}



//
// Scroll intro text across the display. If abortable is set 
// then return true if fire is pressed
//
boolean ScrollMessage(char *msg, boolean abortable=true) {
  int      shift;
  byte     rnd;
  
  qk.Clear();
  shift=0;
  rnd=0;
  while (!qk.DrawText(msg, ArialBold_16, shift++)) {
    rnd+=random(0,255);
    qk.Flip();
    qk.Clear();
    qk.Delay(MESSAGESPEED);
    if (abortable && qk.ReadFireButton()) return true;   
  }
  return false;
}



void setup() {
  randomSeed(analogRead(0));
  IntroScreen();
  PlayTune(korobeiniki,KOROBEINIKISPEED);
  qk.Clear();
  qk.Flip();

}



void loop() {
  boolean  gameover;
  byte     level;
  byte     oldLevel;
  int      lines;
  int      score;
  byte     nextPiece;
  byte     piece;
  byte     pieceX;
  byte     pieceY;
  byte     pieceR;
  boolean  collision;
  byte     buttons;
  boolean  fire;
  byte     buttonPressed;
  int      tmr;
  byte     tmp;
  byte     speed;
  byte     freefallCnt;
  
  while (!ScrollMessage("   Press fire"));
  
  qk.Clear();
  qk.Line(0,0, 0,15);
  qk.Line(11,0, 11,15);
  Draw3x5Digit(0, 13,5);
  Draw3x5Digit(0, 13,11);
  qk.Flip();
  
  nextPiece=random(0,7);
  oldLevel=0;
  lines=0;
  score=0;
  gameover=false;

  while (!gameover) {
    // Calculate current level
    level=min(9,1+lines/10);
    
    // Calculate current speed in 10mS units 
    speed=(11-level)*5;

    // Show level-message if new level
    if (level!=oldLevel) {
      oldLevel=level;
      sprintf(tmps,"    Level %d", level);
      ScrollMessage(tmps,false);    
      qk.Clear();
      qk.Line(0,0, 0,15);
      qk.Line(11,0, 11,15);
      Draw3x5Digit(level, 13,11);
      Draw3x5Digit(lines%10, 13,5);
      qk.Flip();
    }
    
    freefallCnt=0;
    piece=nextPiece;
    pieceX=3;
    pieceY=-1;
    pieceR=0;
    nextPiece=random(0,7);
    PreviewPiece(nextPiece);

    if (IsPieceCollision(piece, pieceX, pieceY, pieceR)) {
      noTone(8); 
      tone(8, 700, 500); 
      qk.Delay(600);
      gameover=true;
      continue;
    }

    DrawPiece(piece, pieceX, pieceY, pieceR);
    qk.Flip();
  
    collision=false;
    tmr=speed;
    while (!collision) {
      qk.Delay(10);

      buttons=qk.ReadDirButtons();
      fire=qk.ReadFireButton();
      if ((buttons==0) && (!fire)) buttonPressed=false;

      if ((buttons&BUTTON_LEFT) && (!buttonPressed)) {
        buttonPressed=true;
        UndrawPiece(piece, pieceX, pieceY, pieceR);
        if (!IsPieceCollision(piece, pieceX-1, pieceY, pieceR)) {
          pieceX--;
          DrawPiece(piece, pieceX, pieceY, pieceR);
          qk.Flip();
        }
      }

      if ((buttons&BUTTON_RIGHT) && (!buttonPressed)) {
        buttonPressed=true;
        UndrawPiece(piece, pieceX, pieceY, pieceR);
        if (!IsPieceCollision(piece, pieceX+1, pieceY, pieceR)) {
          pieceX++;
          DrawPiece(piece, pieceX, pieceY, pieceR);
          qk.Flip();
        }
      }

      if ((buttons&BUTTON_DOWN) && (!buttonPressed)) {
        buttonPressed=true;
        UndrawPiece(piece, pieceX, pieceY, pieceR);
        if (!IsPieceCollision(piece, pieceX, pieceY, (pieceR+1)&3)) {
          pieceR=(pieceR+1)&3;
          DrawPiece(piece, pieceX, pieceY, pieceR);
          qk.Flip();
        }
      }


      if ((fire) && (!buttonPressed)) {
        buttonPressed=true;
        speed=DROPSPEED;
        tmr=0;
      }

      if (tmr-- < 0) {
        if (speed!=DROPSPEED) freefallCnt++;
        tmr=speed;
        DrawPiece(piece, pieceX, pieceY, pieceR);
        qk.Flip();
        UndrawPiece(piece, pieceX, pieceY, pieceR);
        collision=IsPieceCollision(piece, pieceX, pieceY+1, pieceR);
        if (collision) {
          DrawPiece(piece, pieceX, pieceY, pieceR);
          score=((21+(3*level))-freefallCnt);
          tmp=CompletedLines();
          if (tmp>0) {
            lines+=tmp;
            Draw3x5Digit(lines%10, 13,5);
          }
        }
        pieceY++;
      }
    }
  }

  for (tmp=0; tmp<20; tmp++) {
    sprintf(tmps,"    Game over... Score %d at Level %d", score,level);
    if (ScrollMessage(tmps,tmp==0)) break;
  }    

}





