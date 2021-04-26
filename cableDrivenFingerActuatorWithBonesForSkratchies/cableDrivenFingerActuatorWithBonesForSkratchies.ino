#include <DRV8835MotorShield.h>

#define LED_PIN 13


DRV8835MotorShield motors;
int POT_PIN = A4;

// FLEX SENSOR setup
const int FLEX_PIN = A1; // Pin connected to voltage divider flex sensor output
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE = 29800.0; // resistance when straight
const float BEND_RESISTANCE = 67000.0; // resistance at 90 deg

int potValue; //potentiometer value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  potValue = analogRead(POT_PIN);  // read potentiometer value
  int motorSpeed = convertPotentiometerToMotorSpeed (potValue);
  setMotorSpeed(motorSpeed);

  int angle = senseBending();
}


void setMotorSpeed(int motorSpeed){
  motors.setM1Speed(motorSpeed);
}

int convertPotentiometerToMotorSpeed (int potValue){
  int SpeedForMotor;
  if (potValue > 510){
    SpeedForMotor = map(potValue, 511, 1021, 0, 400);
  }
 else{
  SpeedForMotor = map(potValue, 0, 510, -400, 0);
}
  //Serial.println(SpeedForMotor);
  return SpeedForMotor;
}

float senseBending(){
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  //Serial.println("Resistance: " + String(flexR) + " ohms");

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0.0, 90.0);
  //Serial.println("Bend: " + String(angle) + " degrees");
  Serial.println(angle);
  return angle;
}
