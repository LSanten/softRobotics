#include <DRV8835MotorShield.h>
#include <Adafruit_NeoPixel.h>


#define LED_PIN 12
#define LED_COUNT 5

// FLEX SENSOR setup
const int FLEX_PIN = A1; // Pin connected to voltage divider flex sensor output
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE = 29800.0; // resistance when straight
const float BEND_RESISTANCE = 67000.0; // resistance at 90 deg

uint32_t lastStatustMillis = 0;
bool pumpStatus = false;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

DRV8835MotorShield motors;

void setup() {
  
  strip.begin();// NEOPIXEL setup
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
  
  pinMode(FLEX_PIN, INPUT);

}

void loop() {
  uint32_t currentMillis = millis(); // assign loop millisecond time to variable
  
  float angle = senseBending();
  adjustLightToInput(angle);
  //pump(currentMillis);

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

void adjustLightToInput(float angle){
  float intensity = (angle) / 100;
  int R = 255 * intensity;
  int G = 20 * intensity;
  int B = 147 * intensity;
  uint32_t colorStrip = strip.Color(R,G,B);
  strip.fill(colorStrip, 0, 10);
  strip.show();
}

void pump(uint32_t currentMillis){
  int secondsCount = currentMillis / 5000;
  if (secondsCount % 2 == 0){
    int speed = 400;
    motors.setM1Speed(speed); 
  }
  else{motors.setM1Speed(0);}
   
}
