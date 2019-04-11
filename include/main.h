#include <16F887.h>
#device ADC=10

#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG

#use delay(internal=8000000,clock_out)

#use PWM(OUTPUT=PIN_C1, FREQUENCY=30kHz, DUTY=50)

#use rs232( baud=9600, parity=N, xmit=PIN_C6, rcv=PIN_C7, bits=8, ERRORS)

#include "lcd16216.c"
#include "INPUT.c"



