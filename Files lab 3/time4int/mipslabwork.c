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
int timeoutcount = 0;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  if(IFS(0) & 0x100)
  {
    IFSCLR(0) = 0x100;
    timeoutcount++;

    if(timeoutcount == 10)
    {
      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      tick(&mytime);
    }
  }

  if(IFS(0) & (1 << 11)) //checks if an interrup happens
  {
    mytime += 0x2;
    IFSCLR(0) = 0x800; //bit number 11, we check 11 in if.
    time2string(textstring, mytime);
    display_string(3, textstring);
    display_update();
  }
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  //trisE is set to a volatile pointer with the adress specified in the assignment
  volatile int *trisE = (volatile int*) 0xbf886100;
  //Set LEDs as the output (trisE bit 0-7), so and them with 0x0. The others we leave as they are so and with 1111 1111
  //The used Port E bits are the ones set to 0 by the tris.
  *trisE = *trisE & 0xFF00;

  //bits 11 through 5 of port D are set as inputs.
  TRISDSET = 0xFE0;

  PR2 = (80000000 / 256) / 10; // 80 million comes from that timer2 operates at 80 MHz. After /256 / 10 we get 31250 Hz. KOLLA SLIDE 10 PRESENATION 6
  T2CONSET = 0x70; //Setting prescale
  TMR2 = 0;
  T2CONSET = 0x8000; //turn timer on. set bit 15 to 1

  //set interrupts things enables the interrups on tmr2 and sw2. The last thing is to use the function/method from labwork.S
  IPC(2) = IPC(2) | 0x10; //timer 2
  IEC(0) = 0x100; //set bit number 8 to 1 to enable interrupts
  IECSET(0) = 0x800; //int2, bit 11
  IPCSET(2) = 0x1c000000; //set priority of switch 2 to 111
  enable_interrupt();
  return;
}

/* This function is called repetitively from the main program */
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
  prime = nextprime(prime);
  display_nextprime(prime);
  display_string(0, itoaconv(prime));
  display_update();
}
