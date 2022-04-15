#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "peripherals.h"

void displayTime(unsigned long);
void displayTemp(float);
void runTimerA2(void);
void stopTimerA2(int);
void rotatingDisplay();

unsigned long timerCount = 0;;

// Main
void main(void){
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!

    // Useful code starts here
    _BIS_SR(GIE); // Enable Interrupts

    // Set Timer Count to Birthday
    // 304 days till Nov
    // 8 days from Nov 1 to Nov 9
    timerCount = 312 * (24 * 60 * 60);
    runTimerA2();

    while (1) {
        rotatingDisplay(timerCounts, 51.2);
    }
}

void rotatingDisplay(unsigned long counts, float avgTempC){
    displayTimer(counts);
    displayTemp(avgTempC);
}

// Date MMM DD
// HH.MM.SS
void displayTime(unsigned long counts){
    char date[6];
    char time[8];
    time[2] = '.';
    time[5] = '.';
    date[4] = ' ';

    int month = 1;
    int days = 1;
    int hours = 0;
    int minutes = 0;

    int fullDays = seconds / (24*60*60);
    seconds -= fullDays * (24*60*60);
    hours = seconds  / (60 * 60);
    seconds -= hours * 60 * 60;
    minutes = seconds / 60;
    seconds -=  minutes * 60;

    if(fullDays < 31){ // January
        month = 1;
        date[0] = 'J';
        date[1] = 'A';
        date[2] = 'N';
    } else if(fullDays < 59){
        month = 2;
        date[0] = 'F';
        date[1] = 'E';
        date[2] = 'B';
        day = fullDays - 31 ;
    } else if(fullDays < 90){
        month = 3;
        date[0] = 'M';
        date[1] = 'A';
        date[2] = 'R';
       day = fullDays - 59;
    } else if(fullDays < 120){
        month = 4;
        date[0] = 'A';
        date[1] = 'P';
        date[2] = 'R';
        day = fullDays - 90;
    } else if(fullDays < 151){
        month = 5;
        date[0] = 'M';
        date[1] = 'A';
        date[2] = 'Y';
        day = fullDays - 120;
    } else if(fullDays < 181){
        month = 6;
        date[0] = 'J';
        date[1] = 'U';
        date[2] = 'N';
        day = fullDays - 151;
    } else if(fullDays < 212){
        month = 7;
        date[0] = 'J';
        date[1] = 'U';
        date[2] = 'L';
        day = fullDays - 181;
    } else if(fullDays < 243){
        month = 8;
        date[0] = 'A';
        date[1] = 'U';
        date[2] = 'G';
        day = fullDays - 212;
    } else if(fullDays < 273){
        month = 9;
        date[0] = 'S';
        date[1] = 'E';
        date[2] = 'P';
        day = fullDays - 243;
    } else if(fullDays < 304){
        month = 10;
        date[0] = 'O';
        date[1] = 'C';
        date[2] = 'T';
        day = fullDays - 273;
    } else if(fullDays < 334){
        month = 11;
        date[0] = 'N';
        date[1] = 'O';
        date[2] = 'V';
        day = fullDays - 304;
    } else if (fullDays < 365){
        month = 12;
        date[0] = 'D';
        date[1] = 'E';
        date[2] = 'C';
        day = fullDays - 334;
    }

    date[5] = days % 10 + 0x30;
    days /= 10;
    date[4] = days % 10 + 0x30;

    time[1] = hours % 10 + 0x30;
    hours /= 10;
    time[0] = hours % 10 + 0x30;

    time[4] = minutes % 10 + 0x30;
    minutes /= 10;
    time[3] = minutes % 10 + 0x30;

    time[7] = seconds % 10 + 0x30;
    seconds /= 10;
    time[6] = seconds % 10 + 0x30;

    int currentTimer = timerCounts;

    Graphics_drawStringCentered(&g_sContext, date, 6, 45, 5, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

    while(timerCounts < currentTimer + 3);
    currentTimer = timerCounts;
    // delay 3 seconds

    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext, time, 8, 45, 5, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

    while(timerCounts < currentTimer + 3);
    Graphics_clearDisplay(&g_sContext);
}

// ddd.f C
// ddd.f F
void displayTemp(float inAvgTempC){
    char faren[7];
    char cels[7];
    faren[3] = '.';
    cels[3] = '.';
    faren[5] = ' ';
    cels[5] = ' ';
    faren[6] = 'F';
    cels[6] = 'F';

    int celsTrunc = inAvgTempC * 10;
    float celsToFaren = 1.8 * inAvgTempC + 32;
    int farenTrunc = celsToFaren * 10;

    cels[4] = celsTrunc % 10 + 0x30;
    celsTrunc /= 10;
    cels[2] = celsTrunc % 10 + 0x30;
    celsTrunc /= 10;
    cels[1] = celsTrunc % 10 + 0x30;
    celsTrunc /= 10;
    cels[0] = celsTrunc % 10 + 0x30;


    faren[4] = farenTrunc % 10 + 0x30;
    farenTrunc /= 10;
    faren[2] = farenTrunc % 10 + 0x30;
    farenTrunc /= 10;
    faren[1] = farenTrunc % 10 + 0x30;
    farenTrunc /= 10;
    faren[0] = farenTrunc % 10 + 0x30;

    int currentTimer = timerCounts;

    Graphics_drawStringCentered(&g_sContext, cels, 7, 48, 45, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

    while(timerCounts < currentTimer + 3);
    currentTimer = timerCounts;

    // wait 3 seconds

    Graphics_drawStringCentered(&g_sContext, faren, 7, 48, 45, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

    while(timerCounts < currentTimer + 3);
    Graphics_clearDisplay(&g_sContext);
}

// 1 Second
void runTimerA2(void) {
  TA2CTL = TASSEL_1 + MC_1 + ID_3;
  TA2CCR0 = 4095;       // 4095 + 1 = 4096 ACLK tics = 1 seconds
  TA2CCTL0 = CCIE;     // TA2CCR0 interrupt enabled
}

void stopTimerA2(int reset) {
  TA2CTL = MC_0;        // stop timer
  TA2CCTL0 &= ~CCIE;    // TA2CCR0 interrupt disabled
  if(reset)
    timerCount=0;
}

// Timer A2 interrupt service routine
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void) {
  if(tdir) {
    timerCount++;
  }
  else
    timerCount--;
}
