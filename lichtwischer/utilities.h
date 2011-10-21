/**********************************************************************
 * Collection of miscellaneous helper functions
 */

#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#define SEQ_MAX 8

typedef struct {
  int size;
  int fill;
  int head;
  float maximum;
  float minimum;
  float sum;
  float average;
  float values[SEQ_MAX];
} sSeq;

void seq_init( sSeq& seq, int size ) {
  seq.size = size;
  seq.fill = 0;
  seq.head = 0;
  float maximum = 0.0;
  float minimum = 0.0;
  float sum = 0.0;
  float average = 0.0;
}

void seq_push( sSeq& seq, float value ) {
  seq.values[seq.head] = value;
  if( seq.fill < seq.size ) {
    seq.fill++;
  }
  seq.head = (seq.head + 1) % seq.size;
}

float seq_stats( sSeq& seq ) {
  
  switch( seq.fill ) {
  
    case 0: // cave: undefined  
      seq.maximum = 0.0;
      seq.minimum = 0.0;
      seq.sum = 0.0;
      seq.average = 0.0;
      break;
      
    case 1:
      seq.maximum = seq.values[0];
      seq.minimum = seq.values[0];
      seq.sum = seq.values[0];
      seq.average = seq.values[0];
      break;

    default:
      if( seq.values[0] > seq.values[1] ) {
        seq.maximum = seq.values[0];
        seq.minimum = seq.values[1];
      } else {
        seq.maximum = seq.values[1];
        seq.minimum = seq.values[0];
      }
      seq.sum = seq.values[0] + seq.values[1];
      int i;
      for( i=2 ; i<seq.fill ; i++ ) {
        seq.sum += seq.values[i];
        if( seq.values[i] > seq.maximum ) seq.maximum = seq.values[i];
        else if( seq.values[i] < seq.minimum ) seq.minimum = seq.values[i];
      }
      seq.average = seq.sum / seq.fill;
      break;
        
  }
}

#endif // __UTILITIES_H_

