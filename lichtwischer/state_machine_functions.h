/**********************************************************************
 * State machine functions
 */

#ifndef __STATE_MACHINE_FUNCTIONS_H_
#define __STATE_MACHINE_FUNCTIONS_H_

#include "sensor.h"

// Swing state machine states
// cave: used for array indexing
#define SWING_IDLE 0
#define SWING_POSITIVE 1
#define SWING_POSITIVE_MAX 2
#define SWING_POSITIVE_ZERO 3
#define SWING_NEGATIVE 4
#define SWING_NEGATIVE_MAX 5
#define SWING_NEGATIVE_ZERO 6

#define SWING_THRESHOLD 100
#define SWING_IDLE_TIMEOUT 300000

int swingState;
long swingStateTime[8];

void set_state( int state ) {
  swingState = state;
  swingStateTime[state] = micros();
}

int state_is( int state ) {
  return swingState == state;
}

int positive_threshold_passed() {
  return( (sensorPrevX < SWING_THRESHOLD) && (sensorX >= SWING_THRESHOLD) );
}

int positive_max_passed() {
  return( sensorPrevX > sensorX );
}

int positive_zero_passed() {
  return( (sensorPrevX > 0) && (sensorX <= 0) );
}

int negative_threshold_passed() {
  return( (sensorPrevX > -SWING_THRESHOLD) && (sensorX <= -SWING_THRESHOLD) );
}

int negative_max_passed() {
  return( sensorPrevX < sensorX );
}

int negative_zero_passed() {
  return( (sensorPrevX < 0) && (sensorX >= 0) );
}

int idle_timeout() {
  return( micros() - swingStateTime[swingState] >= SWING_IDLE_TIMEOUT );
}

#endif // __STATE_MACHINE_FUNCTIONS_H_
