#include <Arduino.h>
const int encoderPinA = 2; 
const int PULSES_PER_REV = 255;  
unsigned long lastTime = 0;

const int RPWM = 12; //forward PWM 
const int LPWM = 13; // Reverse PWM
const int ENABLE = 28;
const int REVERSE = 30;
//--- Ultrasonic Sensor A ---
const int echoPinA = 10;

// --- Ultrasonic Sensor B ---
const int trigPinA = 7;
const int trigPinB = 7; 
const int echoPinB = 9;

//Variables
long durationA, durationB;
int distanceA, distanceB;


volatile long encoderCount = 0;

// Interrupt Service Routine
void encoderISR() {
  // Read A channel to know direction
  if (digitalRead(encoderPinA) == HIGH) {
    encoderCount++;   // one direction
  } else {
    encoderCount--;   // opposite direction
  }
}
void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  //pinMode(encoderPinB, INPUT_PULLUP);

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(REVERSE, OUTPUT);

  digitalWrite(ENABLE, HIGH); // enable the motor driver
  digitalWrite(REVERSE, HIGH);  // set direction to forward
  pinMode(trigPinA, OUTPUT);
  pinMode(echoPinA, INPUT);

  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, RISING);
}



int readDistance(int trigPin, int echoPin) {
  // Clear trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10 µs pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo
  long duration = pulseIn(echoPin, HIGH);

  // Convert to cm
  int distance = duration * 0.034 / 2;

  return distance;
}


void runMotor(int speed, bool forward) {
    speed = constrain(speed, 0, 255);
    if (forward) {
        analogWrite(RPWM, speed);
        analogWrite(LPWM, 0);
    } else {
        analogWrite(LPWM, speed);
        analogWrite(RPWM, 0);
    }

    noInterrupts();
    long count = encoderCount;
    interrupts();

    if (count >= PULSES_PER_REV) {
        Serial.println("One revolution complete");
        encoderCount = 0;
        analogWrite(RPWM, 0);
        analogWrite(LPWM, 0);
        delay(1000); // pause between revolutions
    }
}

void loop() {
  distanceA = readDistance(trigPinA, echoPinA);
  distanceB = readDistance(trigPinB, echoPinB);

  // Difference
  int diff = abs(distanceA - distanceB);
  int speed = diff*5;
  Serial.println("Distance A: " + String(distanceA) + " cm");
  Serial.println("Distance B: " + String(distanceB) + " cm");
  Serial.println("Difference: " + String(diff) + " cm");
  Serial.println("speed"+ speed);
  // Forward flag depends on which distance is larger
  bool forward = (distanceA > distanceB);
  
  runMotor(speed, forward);
  
  
}

