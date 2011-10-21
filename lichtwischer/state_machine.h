/**********************************************************************
 * Main state machine handler
 * Has to be separated here because it's interwoven with anim.h
 */

#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_

#include "state_machine_functions.h"
#include "anim.h"

void state_machine() {

  switch( swingState ) {

    case SWING_IDLE:
      if( positive_threshold_passed() ) set_state( SWING_POSITIVE );
      else if( negative_threshold_passed() ) set_state( SWING_NEGATIVE );
      anim_reset();
      delay(10);
      break;

    case SWING_POSITIVE:
      if( positive_max_passed() ) {
        set_state( SWING_POSITIVE_MAX );
        right_frame_sync();
      } 
      break;

    case SWING_POSITIVE_MAX:
      if( positive_zero_passed() ) {
        set_state( SWING_POSITIVE_ZERO );
      }
      break;

    case SWING_POSITIVE_ZERO:
      if( idle_timeout() ) set_state( SWING_IDLE );
      else if( negative_threshold_passed() ) {
        set_state( SWING_NEGATIVE );
      }
      break;

    case SWING_NEGATIVE:
      if( negative_max_passed() ) {
        set_state( SWING_NEGATIVE_MAX );
        left_frame_sync();
      } 
      break;

    case SWING_NEGATIVE_MAX:
      if( negative_zero_passed() ) {
        set_state( SWING_NEGATIVE_ZERO );
      }
      break;

    case SWING_NEGATIVE_ZERO:
      if( idle_timeout() ) set_state( SWING_IDLE );
      else if( positive_threshold_passed() ) {
        set_state( SWING_POSITIVE );
      }
      break;

  }

}

#endif // __STATE_MACHINE_H_
