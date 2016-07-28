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
  // the default timeout for pulseIn is 1 second, a long time to wait for each loop
  // when a wire breaks or something.
  int ch1_pulse = pulseIn( ch1, HIGH, 40000 );
  int ch2_pulse = pulseIn( ch2, HIGH, 40000 );
  int ch3_pulse = pulseIn( ch3, HIGH, 40000 );
  float normalized_forward = 0.0;
  float normalized_rotation = 0.0;
  float normalized_velocity_right = 0.0;
  float normalized_velocity_left = 0.0;

  // On the AMT Class stock radio, channel 3 toggles between 980 uS and 1995 uS 
  // on pressing the little button.  Store the state of button before checking again.
  oldWeaponPulseState = weaponPulseState;
  
  forward = conditionPulse( ch1_pulse, deadBand );
  rotation = conditionPulse( ch2_pulse, deadBand );
  weapon = conditionPulse( ch3_pulse, deadBand );

  normalized_forward = normalizePulse( ch1_pulse, deadBand );
  normalized_rotation = normalizePulse( ch2_pulse, deadBand );
  
  if( weapon < 0 ){ weaponPulseState = false; }
  if( weapon > 0 ){ weaponPulseState = true; }
  
  if( weaponPulseState != oldWeaponPulseState ){
    // Do something brutal with this data.
    // Serial.println( "Weapon!" );
  }
  
  // simple differential drive:
  velocity_right = forward - rotation;
  velocity_left = forward + rotation;

  // expo 1.0 is linear, 0.0 to 1.0 is less sensitive at center, 1.0 to 1.5 is more sensitive at center
  normalized_velocity_right = expo( ( normalized_forward - normalized_rotation ), 1.0 );
  normalized_velocity_left = expo( ( normalized_forward + normalized_rotation ), 1.0 );

  // turn off the H Bridge while doing nothing
  if ( velocity_right == 0 && velocity_left == 0 ) {
    standbyHBridge();
  }
  else {
    enableHBridge();
  }

  // constrain the results to +/- 255
  velocity_left = constrain( velocity_left, -255, 255 );
  velocity_right = constrain( velocity_right, -255, 255 );

  // order the H Bridge to control the motors.
  drive( 1, velocity_left );
  drive( 2, velocity_right );

  // You might think to delay 20 mS here, because the the radio receiver gives updates
  // at 50 Hz.  However, the pulseIn code at beginning of loop will wait for a new servo 
  // pulse or the 40 mS timeout.  The loop speed will be controlled by the servo pulses.
}
