/**********************************************************************
 * Sensor handling
 */

#ifndef __SENSOR_H_
#define __SENSOR_H_

// unsure why this is required in an include file
#include <WProgram.h>

#include "utilities.h"

// analog inputs from ADXL330
const int sensorXPin = A0;
const int sensorYPin = A1;
const int sensorZPin = A2;

float sensorX = 0;
float sensorY = 0;
float sensorZ = 0;
float sensorX0 = 0;
float sensorY0 = 0;
float sensorZ0 = 0;
float sensorPrevX = 0;
float sensorPrevY = 0;
float sensorPrevZ = 0;

void sensor_read() {
  float xr, yr, zr;
  sensorPrevX = sensorX;
  sensorPrevY = sensorY;
  sensorPrevZ = sensorZ;
  xr = analogRead( sensorXPin ) - sensorX0;
  yr = analogRead( sensorYPin ) - sensorY0;
  zr = analogRead( sensorZPin ) - sensorZ0;
  // apply 45-degree z rotation matrix
  sensorX = 0.707107 * (xr - yr);
  sensorY = 0.707107 * (xr + yr);
  sensorZ = zr;
}

void sensor_calibrate() {
  int i;
  float x0, y0, z1;
  
  sSeq seqX, seqY, seqZ;
  
  seq_init( seqX, 8 );
  seq_init( seqY, 8 );
  seq_init( seqZ, 8 );

  for( i=8 ; i>0 ; i-- ) {
    seq_push( seqX, analogRead( sensorXPin ) );
    seq_push( seqY, analogRead( sensorYPin ) );
    seq_push( seqZ, analogRead( sensorZPin ) );
    delay( 10 );
  }

  seq_stats( seqX );
  seq_stats( seqY );
  seq_stats( seqZ );

  x0 = seqX.average;
  y0 = seqY.average;
  z1 = seqZ.average;
  
  // set preliminary zero point so we can use sensor_read()
  sensorX0 = x0;
  sensorY0 = y0;
  sensorZ0 = z1; // CAVE: z measures g!
 
}

#endif // __SENSOR_H_
