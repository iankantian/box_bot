/*
 * box_bot.ino
 * Joshua Brown 2016
 * for Box Bots at Ace Monster Toys:
 * https://www.acemonstertoys.org/box-bots/
 */
 
#include "box_bot_defs.h"
#include "box_bot.h"

void setup() {
  boxBotInit();
}

void loop() {
  int ch1_pulse = pulseIn( ch1, HIGH );
  int ch2_pulse = pulseIn( ch2, HIGH );
  int ch3_pulse = pulseIn( ch3, HIGH );
  
  forward = conditionPulse( ch1_pulse, deadBand );
  rotation = conditionPulse( ch2_pulse, deadBand );
  weapon = conditionPulse( ch3_pulse, deadBand );

  // reverse steering while backing up, just feels more like driving a car:
  if( forward < -deadBand ){
    rotation = -rotation;
  }
  // simple differential drive:
  velocity_right = forward - rotation;
  velocity_left = forward + rotation;

  if ( velocity_right == 0 && velocity_left == 0 ) {
    standbyHBridge();
  }
  else {
    enableHBridge();
  }

  velocity_left = limitPlusMinus( velocity_left, 255 );
  velocity_right = limitPlusMinus( velocity_right, 255 );
  
  drive( 1, velocity_left );
  drive( 2, velocity_right );
  
  delay( 20 );
}
