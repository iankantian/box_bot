/*
  box_bot.h
  Joshua Brown 2016
  for:
  Box Bot at Ace Monster Toys
  https://www.acemonstertoys.org/box-bots/
*/
void boxBotInit(){
  // just for debugging:
  Serial.begin(9600);

  // pins for the motor controller assigned by the custom circuit board 
  // designed and built by Ray at Ace Monster Toys
  pinMode(pwmB, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(stby, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(pwmA, OUTPUT);
  

  // I chose arbitrarily where to plug the receiver sense pins
  // using on board pull ups in case of wire breakage.
  pinMode(ch1, INPUT_PULLUP);
  pinMode(ch2, INPUT_PULLUP);
  pinMode(ch3, INPUT_PULLUP);

  // get initial weapon  state
  if( pulseIn( ch3, HIGH, 40000 ) > 1500 ){
    weaponPulseState = true;
  }

}

// standbyHBridge derived from truth table of the Toshiba TB6612 dual H-bridge
// datasheet
void standbyHBridge() {
  digitalWrite( stby, LOW );
}

// enableHBridge derived from truth table of the Toshiba TB6612 dual H-bridge
// datasheet
void enableHBridge() {
  digitalWrite( stby, HIGH );
}

int conditionPulse( int pulse, int deadBand ){
  // Check to see if valid pulse ~800 to ~2200 uS.  If not, it should
  // be set to middle: 1500
  if( pulse < 800 || pulse > 2200 ){
    pulse = 1500;
  }
  // turn positve integer into a range of values centered on 0:
  pulse -= 1500;
  //  Serial.print("pulse before conditioning is ");
  //  Serial.println( pulse );
  // remove noise of just sitting still with the controller
  if( pulse > deadBand || pulse < -deadBand ){
      // result fits nicely under 255 by dividing by 2
     pulse /= 2;
  }
  else{
      pulse = 0;
  }
  return pulse;
}

// turn the pulse into a float value -1.0 to 1.0
float normalizePulse( int pulse, int deadBand ){
  float norm_pulse = 0.0;
  // Check to see if valid pulse ~800 to ~2200 uS.  If not, it should
  // be set to middle: 0.5
  if( pulse < 800 || pulse > 2200 ){
    norm_pulse = 0.5;
  }
  // turn positve integer into a range of values centered on 0:
  pulse -= 1500;
  //  Serial.print("pulse before conditioning is ");
  //  Serial.println( pulse );
  // remove noise of just sitting still with the controller
  if( pulse > deadBand || pulse < -deadBand ){
     norm_pulse = pulse / 500.0;
  }
  else{
      norm_pulse = 0.0;
  }
  return norm_pulse;
}

float expo( float input, float factor ){
  // factor of 1.0 is full linear, 0.0 to 1.0 is less sensitive in center.
  // 1.0 to 1.5 is the opposite, where the sensitivity is greater at center and less and endpoints.
  // factors outside of 0 to 1.5 lead to undershooting or overshooting the output, which should
  // range from -1.0 to 1.0.
  // thanks to user "rcgdlr" at https://www.physicsforums.com/threads/equation-required-to-calculate-exponential-rate.524002/
  // for this equation.
  return( ( ( 1 - factor ) * input * input * input )  + ( factor * input ) );
}

// limit helper function
int limitPlusMinus( int input, int limit ){
  if ( input > limit ) {
    input = limit;
  }
  if ( input < -limit ) {
    input = -limit;
  }
  return input;
}

// drive function is derived by the truth table of the 
// Toshiba TB6612 Dual H-Bridge Driver
void drive( int motor, int velocity ) {
  if ( motor == 1 ) {
    // going backwards
    if ( velocity < 0 ) {
      analogWrite( pwmA, -velocity );
      digitalWrite( ain1, LOW );
      digitalWrite( ain2, HIGH );
    }
    // going forwards
    else if ( velocity > 0 ) {
      analogWrite( pwmA, velocity );
      digitalWrite( ain1, HIGH );
      digitalWrite( ain2, LOW );
    }
    // going nowhere
    else {
      // brakes on!
      digitalWrite( ain1, HIGH );
      digitalWrite( ain2, HIGH );
    }
  }
  else if ( motor == 2) {
    if ( velocity < 0 ) {
      analogWrite( pwmB, -velocity );
      digitalWrite( bin1, LOW );
      digitalWrite( bin2, HIGH );
    }
    else if ( velocity > 0 ) {
      analogWrite( pwmB, velocity );
      digitalWrite( bin1, HIGH );
      digitalWrite( bin2, LOW );
    }
    else {
      digitalWrite( bin1, HIGH );
      digitalWrite( bin2, HIGH );
    }
  }
  else {
    // error message
    //Serial.println("unexpected motor");
  }
}

// Toshiba TB6612 Dual H-Bridge Driver
void driveNormalized( int motor, float velocity ) {
  if ( motor == 1 ) {
    // going backwards
    if ( velocity < 0 ) {
      analogWrite( pwmA, ( int )constrain( -velocity * 255, -255, 255 ) );
      digitalWrite( ain1, LOW );
      digitalWrite( ain2, HIGH );
    }
    // going forwards
    else if ( velocity > 0 ) {
      analogWrite( pwmA, ( int )constrain( velocity * 255, -255, 255 ) );
      digitalWrite( ain1, HIGH );
      digitalWrite( ain2, LOW );
    }
    // going nowhere
    else {
      // brakes on!
      digitalWrite( ain1, HIGH );
      digitalWrite( ain2, HIGH );
    }
  }
  else if ( motor == 2) {
    if ( velocity < 0 ) {
      analogWrite( pwmB, ( int )constrain( -velocity * 255, -255, 255 ) );
      digitalWrite( bin1, LOW );
      digitalWrite( bin2, HIGH );
    }
    else if ( velocity > 0 ) {
      analogWrite( pwmB, ( int )constrain( velocity * 255, -255, 255 ) );
      digitalWrite( bin1, HIGH );
      digitalWrite( bin2, LOW );
    }
    else {
      digitalWrite( bin1, HIGH );
      digitalWrite( bin2, HIGH );
    }
  }
  else {
    // error message
    //Serial.println("unexpected motor");
  }
}
