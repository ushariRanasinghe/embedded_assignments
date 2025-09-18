#include <Arduino.h>

const int RPWM = 12; //forward PWM 
const int LPWM = 13; // Reverse PWM
const int ENABLE = 28;
const int REVERSE = 30;
void setup() {
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(REVERSE, OUTPUT);
  digitalWrite(ENABLE, HIGH); // enable the motor driver
  digitalWrite(REVERSE, HIGH);  // set direction to forward
}

void loop() {
  // put your main code here, to run repeatedly:
  //forward slow 
  analogWrite(RPWM, 100);
  analogWrite(LPWM, 0);
  delay(2000);
  //forward fast
  analogWrite(RPWM, 500);
  analogWrite(LPWM, 0);
  delay(2000);
  //stop
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
  delay(2000);
  //backwardslow
  analogWrite(LPWM, 100);
  analogWrite(RPWM, 0);
  delay(2000);
  // backward fast 
  analogWrite(LPWM, 500);
  analogWrite(RPWM, 0);
  delay(2000);
  // stop 
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
  delay(2000);
}