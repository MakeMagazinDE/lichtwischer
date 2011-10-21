/**********************************************************************
 * Animation handling
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include <avr/pgmspace.h>
#include "anim_data.h"
#include "display.h"
#include "state_machine_functions.h"

const int animXSize = ANIM_XSIZE;
const int animYSize = ANIM_YSIZE;
const int animFrames = ANIM_FRAMES;
const int animLength = ANIM_LENGTH;
int animSequenceCounter = 0;

int animFrame = 0;
int animXPos = 0;

long animPrevBarUpdate = 0;
long animRowDuration = 15625;
long animFrameDuration = 250000;

#define ANIM_FORWARD 0
#define ANIM_BACKWARD 1
char animSwingDirection = ANIM_FORWARD;


void advance_frame() {
  animSequenceCounter = (animSequenceCounter+1) % animLength;
  animFrame = animSequence[animSequenceCounter];
  animXPos = (animSwingDirection == ANIM_FORWARD) ? 0 : (animXSize-1);
}

void advance_row() {
  if( animSwingDirection == ANIM_FORWARD ) {
    if( animXPos < animXSize - 1 ) animXPos++;
  } else {
    if( animXPos > 0 ) animXPos--;
  }
}

void anim_reset() {
  animSequenceCounter = 0; 
}

int anim_read_current_row() {
  return pgm_read_word( pgmAnimData
    + animFrame*animXSize + animXPos ); 
}

void anim_update_display() {
  display_set( anim_read_current_row() );
  animPrevBarUpdate = micros();
}

void anim_clear_display() {
  display_clear();
  animPrevBarUpdate = micros();
}

int anim_display_update_required() {
  return( (micros()-animPrevBarUpdate) >= animRowDuration );
}

void anim_handle_display() {
  if( !state_is( SWING_IDLE ) ) {
    if( anim_display_update_required() ) {
      anim_update_display();
      advance_row();
    }
  } else {
    display_clear();
  }
}


int left_frame_sync() {
   animFrameDuration = abs(swingStateTime[SWING_POSITIVE_MAX] - swingStateTime[SWING_NEGATIVE_MAX]);
   animRowDuration = 0.99 * animFrameDuration / animXSize;
   animSwingDirection = ANIM_FORWARD;
   advance_frame();
   // force update on next state cycle
   animPrevBarUpdate = 0;
}

int right_frame_sync() {
   animFrameDuration = abs(swingStateTime[SWING_POSITIVE_MAX] - swingStateTime[SWING_NEGATIVE_MAX]);
   animRowDuration = 0.99 * animFrameDuration / animXSize;
   animSwingDirection = ANIM_BACKWARD;
   advance_frame();
   // force update on next state cycle
   animPrevBarUpdate = 0;
}

#endif // __ANIM_H_
