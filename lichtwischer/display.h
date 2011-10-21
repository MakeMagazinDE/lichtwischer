/**********************************************************************
 * Display handling
 */

#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#define DISPLAY_LED_COUNT 10

const char displayLEDPins[DISPLAY_LED_COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

void display_setup() {
  int led;
  for( led=0 ; led<DISPLAY_LED_COUNT ; led++ ) {
    pinMode( displayLEDPins[led], OUTPUT );
    digitalWrite ( displayLEDPins[led], LOW );
  }
}

void display_set( int value ) {
  int led;
  int bitmask;
  for( led=0, bitmask=1 ; led<DISPLAY_LED_COUNT ; led++, bitmask*=2 ) {
    if( value&bitmask ) {
      digitalWrite( displayLEDPins[led], HIGH );
    } else {
      digitalWrite( displayLEDPins[led], LOW );
    }
  }
}

void display_clear() {
  int led;
  for( led=0 ; led<DISPLAY_LED_COUNT ; led++ ) {
    digitalWrite( displayLEDPins[led], LOW );
  }
}

void display_swipe_up( int ms_delay ) {
  int led;
  for( led=0 ; led<DISPLAY_LED_COUNT ; led++ ) {
    digitalWrite( displayLEDPins[led], HIGH );
    delay( ms_delay );
    digitalWrite( displayLEDPins[led], LOW );
  }
}

void display_swipe_down( int ms_delay ) {
  int led;
  for( led=DISPLAY_LED_COUNT-1 ; led>=0 ; led-- ) {
    digitalWrite( displayLEDPins[led], HIGH );
    delay( ms_delay );
    digitalWrite( displayLEDPins[led], LOW );
  }
}

#endif // __DISPLAY_H_
