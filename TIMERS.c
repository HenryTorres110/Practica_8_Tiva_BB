#include "lib/include.h"
extern void TIMER_CONFIGURATION(int seconds, int f_clk){
    // Auxiliary LED
    SYSCTL -> RCGCGPIO |= (1 << 5); // Enable clock port F
    GPIOF -> AFSEL &= ~(1 << 2); // Disable alternate function on PF2
    GPIOF -> DIR = (1 << 2); // make PF2 output
    GPIOF -> DEN = (1 << 2); 
    GPIOF -> DATA |= (1 << 2); // PF2 set on High

    SYSCTL -> RCGCTIMER |= (1 << 0); // 16/32-Bit General-Purpose Timer 0 Run Mode Clock Gating Control

    // One Shot/ Periodic Timer Mode
    TIMER0 -> CTL &= ~(1 << 0); // Disable timer 0 before making any changes
    TIMER0 -> CFG = 0x00000000;
    TIMER0 -> TAMR |= (0x2 << 0); // Periodic Mode
    TIMER0 -> TAMR &= ~(1 << 4); // count down mode, for count up set bit 4 to High
    TIMER0 -> TAILR = f_clk * seconds;
    TIMER0 -> CTL |= (1 << 0); // Enable timer 0 to start counting

}