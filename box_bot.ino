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
  float normalized_forward = 0.0;
  float normalized_rotation = 0.0;
  float normalized_velocity_right = 0.0;
  float normalized_velocity_left = 0.0;
  
  // the default timeout for pulseIn is 1 second, a long time to wait for each loop
  // when a wire breaks.  So I put in 40 ms as the third argument.
  int ch1_pulse = pulseIn( ch1, HIGH, 40000 );
  int ch2_pulse = pulseIn( ch2, HIGH, 40000 );
  if( ch1_pulse == 0 || ch1_pulse > 2500 ){ ch1_pulse = 1500; }
  if( ch2_pulse == 0 || ch2_pulse > 2500 ){ ch2_pulse = 1500; }
  // if( ch3_pulse == 0 || ch3_pulse > 2500 ){ ch3_pulse = 1500; }


  // apply some 'exponential' to steering channel, 1.0 is full linear, 0.0 to 1.0 is 
  // +expo and 1.0 to 1.5 is -expo.
  normalized_rotation = expo( normalizePulse( ch1_pulse, deadBand ), 0.5 );
  normalized_forward = normalizePulse( ch2_pulse, deadBand );

  // simple differential drive:
  normalized_velocity_right = ( normalized_rotation - normalized_forward );
  normalized_velocity_left = ( normalized_forward  + normalized_rotation );

  // turn off the H Bridge while doing nothing
  if ( normalized_velocity_right == 0 && normalized_velocity_left == 0 ) {
    standbyHBridge();
  }
  else {
    enableHBridge();
  }

  // the timing of the Rx servo pulses make a kind of schedule for updating the 
  // the rates of the motor.  Otherwise the changes would be essentially instantaneous.
  if( abs( normalized_velocity_left - currentLeft ) > slewRate ){
      if( normalized_velocity_left > currentLeft ){ currentLeft += slewRate; }
      else if( normalized_velocity_left < currentLeft ){ currentLeft -= slewRate; }
  }
  else{ 
    currentLeft = normalized_velocity_left;
  }
  if( abs( normalized_velocity_right - currentRight ) > slewRate ){
    if( normalized_velocity_right > currentRight ){ currentRight += slewRate; }
    else if( normalized_velocity_right < currentRight ){ currentRight -= slewRate; }
  }
  else{ 
    currentRight = normalized_velocity_right;
  }

  
  // order the H Bridge to control the motors.
  //driveNormalized( 1, normalized_velocity_left );
  //driveNormalized( 2, normalized_velocity_right );

  driveNormalized( 1, currentLeft );
  driveNormalized( 2, currentRight );

  Serial.print("currentLeft: ");
  Serial.print( currentLeft );
  Serial.println("");

  // You might think to delay 20 mS here, because the the radio receiver gives updates
  // at 50 Hz.  However, the pulseIn code at beginning of loop will wait for a new servo 
  // pulse or the 40 mS timeout.  The loop speed will be controlled by the servo pulses,
  // or lack thereof!
}
