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
  pinMode(ch1, INPUT);
  pinMode(ch2, INPUT);
  pinMode(ch3, INPUT);
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
  // assuming a 1000 uS to 2000 uS input, with 500 uS either side of middle, 
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
