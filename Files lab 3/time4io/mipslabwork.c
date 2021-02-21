/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):
   //Iley Alvarez Funcke 14 February 2021

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  //trisE is set to a volatile pointer with the adress specified in the assignment
  volatile int *trisE = (volatile int*) 0xbf886100;
  //Set LEDs as the output (trisE bit 0-7), so and them with 0x0. The others we leave as they are so and with 1111 1111
  //The used Port E bits are the ones set to 0 by the trisE.
  *trisE = *trisE & 0xFF00;

  //bits 11 through 5 of port D are set as inputs.
  TRISDSET = 0xFE0;
  return;
}

void mytimeCreator(int x)
{
  switch (x)
  {
    case 1:
      mytime = mytime & 0xFF0F;
      mytime = (getsw() << 4) | mytime;
      break;
    case 2:
      mytime = mytime & 0xF0FF;
      mytime = (getsw() << 8) | mytime;
      break;
    case 4:
      mytime = mytime & 0x0FFF;
      mytime = (getsw() << 12) | mytime;
      break;
    default:
      break;
  }
}

//de här är här så att det är globala variabler
int c = 0;
volatile int* portE = (volatile int*) 0xbf886100;

void labwork( void )
{
  //check so it doesn't go over the limit
  if(c == 255)
  {
    c = 0;
  }
  *portE = c;

  switch (getbtns())
  {
    case 1:
      mytimeCreator(1);
      break;
    case 2:
      mytimeCreator(2);
      break;
    case 3:
      mytimeCreator(1);
      mytimeCreator(2);
      break;
    case 4:
      mytimeCreator(4);
      break;
    case 5:
      mytimeCreator(1);
      mytimeCreator(4);
      break;
    case 6:
      mytimeCreator(2);
      mytimeCreator(4);
      break;
    case 7:
      mytimeCreator(1);
      mytimeCreator(2);
      mytimeCreator(4);
      break;
    default:
      break;
  }

  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  *portE = *portE + 1;
  display_image(96, icon);
}
