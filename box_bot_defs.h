/*
  box_bot_defs.h
  Joshua Brown 2016
  for:
  Box Bot at Ace Monster Toys
  https://www.acemonstertoys.org/box-bots/

  The bot controller is Arduino Nano clone, that uses the CH430 serial adapter, so you
  have to figure out how to get that to work with your programming environment.  I used
  http://kig.re/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html
  For original Arduino, it installs automatically with the Arduino IDE:
  https://www.arduino.cc/en/Main/ArduinoBoardNano
  
  The motor driver of the box bot is: 
  http://toshiba.semicon-storage.com/ap-en/product/linear/motordriver/detail.TB6612FNG.html
  and is connected like so:
  D3 pwmB
  D4 bin2
  D5 bin1
  D6 stby
  D7 ain1
  D8 ain2
  D9 pwmA

  The Rx sense pins are connected arbitrarily to: 
  A0 ch1 blue wire
  A1 ch2 brown wire
  A2 ch3 yellow wire
*/

#define pwmB 3
#define bin2 4
#define bin1 5
#define stby 6
#define ain1 7
#define ain2 8
#define pwmA 9

#define ch1 A0
#define ch2 A1
#define ch3 A2

int deadBand = 50;
int velocity_right = 0;
int velocity_left = 0;
int forward;
int rotation;
//int weapon;
//bool weaponPulseState = false;
//bool oldWeaponPulseState = false;

// these variables hold the current state of motor control
float currentLeft = 0.0;
float currentRight = 0.0;
// slewRate is the allowed acceleration, idea being to reduce the amount of 
// impulse (hammering) upon the plastic gearmotors that I do while driving!
// at a slew of 0.1, it will take 10 cycles of the main loop to go from 0 to full
float slewRate = 0.2;




