/********************************************************
 * A little Arduino POV-fu
 * by Christiane Ruetten, cr@23bit.net
 */

#include "sensor.h"
#include "display.h"
#include "state_machine.h"
#include "anim.h"

/**********************************************************************
 * Setup function
 */

void setup() {
  Serial.begin( 115200 );

  pinMode( 12, INPUT );

  display_setup();

  // wait for any button-press movement to settle
  // expect sensor to be sitting flat on a table
  delay( 50 );
  display_swipe_up( 20 );
  sensor_calibrate();
  Serial.print( "Calibrated (0,0,-1) at (" );
  Serial.print( sensorX0 );
  Serial.print(",");
  Serial.print( sensorY0 );
  Serial.print(",");
  Serial.print( sensorZ0 );
  Serial.println(")");
  display_swipe_down( 20 );

  set_state( SWING_IDLE );

/*
 // Sensor sampling test code
  delay(3000);
  int i;
  for( i=0 ; i<10000 ; i++ ) {
    sensor_read();
    Serial.print( micros() );
    Serial.print( " " );
    Serial.print( sensorX );
    Serial.print( " " );
    Serial.print( sensorY );
    Serial.print( " " );
    Serial.println( sensorZ );
  }  
  delay(3000);
*/

}


/**********************************************************************
 * Main loop
 */

void loop() {

  anim_handle_display();

  sensor_read();

  state_machine();
  
}



