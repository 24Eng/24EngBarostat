#include <Adafruit_BMP085.h>

/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These pressure and temperature sensors use I2C to communicate, 2 pins
  are required to interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 peripheralSensor;

// Global variables
long startingPressure = 0;
int threshold = 0;
int previousThreshold = 0;
int spike = 0;
int previousSpike = 0;

long currentPressure = 0;
long previousPressure = 0;
int maxLumens = 1;
int colorHold = 100;
int toneHold = 100;
bool mute = 0;
bool serialComms = 1;

// IO
int redLED = 11;
int grnLED = 10;
int bluLED = 9;
int speaker = 12;
int spikeLevelPotentiometer = A0;
int thresholdPotentiometer = A1;


void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(grnLED, OUTPUT);
  pinMode(bluLED, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(thresholdPotentiometer, INPUT);
  pinMode(spikeLevelPotentiometer, INPUT);
  Serial.begin(115200);
  if (!peripheralSensor.begin()) {
	  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	  while (1) {}
  }
  currentPressure = peripheralSensor.readPressure();
  startingPressure = currentPressure;
  previousPressure = currentPressure;
  if(!mute){
    for(int i=400;i<700;i++){
      tone(speaker, i, 100);
      delay(7-(i/100));
    }
  }
  // noTone(speaker);
}
  
void loop() {
  previousPressure = currentPressure;
  currentPressure = peripheralSensor.readPressure();
  if(serialComms){
    previousThreshold = threshold;
    previousSpike = spike;
  }
  threshold = map(analogRead(thresholdPotentiometer), 0, 1024, 0, 50);
  spike = map(analogRead(spikeLevelPotentiometer), 0, 1024, 0, 50);
  if(serialComms){
    // Serial.print("\n");
    // Serial.print("Pressure: ");
    // Serial.print(currentPressure);
    // Serial.print(" Pa");
    if ((previousSpike != spike) || (previousThreshold != threshold)){
      Serial.print("\n\tSpikePot: ");
      Serial.print(spike);
      Serial.print("\tThresholdPot: ");
      Serial.print(threshold);
    }
  }
  if (currentPressure - previousPressure >= spike){
    alertSpikeUp();
  }else if (previousPressure - currentPressure >= spike){
    alertSpikeDown();
  }else if (currentPressure - startingPressure >= threshold){
    alertThresholdUp();
  }else if ((currentPressure - startingPressure >= threshold) || (previousPressure - startingPressure >= threshold)){
    alertThresholdDown();
  }else{
    turnLEDGrn();
  }
}


// 24 Eng // 24 Eng // 24 Eng // 24 Eng // 24 Eng // 24 Eng // 24 Eng // 24 Eng // 24 Eng 

void alertSpikeUp(){
  turnLEDRed();
  if(serialComms){
    // Serial.print("\tSpikeAlarm↑:\t");
    // Serial.print(currentPressure - previousPressure);
  }
  tone(speaker, 500, toneHold);
  delay(colorHold);
}

void alertSpikeDown(){
  turnLEDBlu();
  if(serialComms){
    // Serial.print("\tSpikeAlarm↓:\t");
    // Serial.print(currentPressure - previousPressure);
  }
  tone(speaker, 500, toneHold);
  delay(colorHold);
}

void alertThresholdUp(){
  turnLEDYlw();
  if(serialComms){
    // Serial.print("\tThresholdAlarm↑:\t");
    // Serial.print(currentPressure - startingPressure);
  }
  tone(speaker, 500, toneHold);
  delay(colorHold);
}

void alertThresholdDown(){
  turnLEDCyn();
  if(serialComms){
    // Serial.print("\tThresholdAlarm↓:\t");
    // Serial.print(currentPressure - startingPressure);
  }
  tone(speaker, 500, toneHold);
  delay(colorHold);
}

void turnLEDGrn(){
  analogWrite(redLED, 0);
  analogWrite(grnLED, maxLumens);
  analogWrite(bluLED, 0);
}
void turnLEDRed(){
  analogWrite(redLED, maxLumens);
  analogWrite(grnLED, 0);
  analogWrite(bluLED, 0);
}
void turnLEDBlu(){
  analogWrite(redLED, 0);
  analogWrite(grnLED, 0);
  analogWrite(bluLED, maxLumens);
}
void turnLEDCyn(){
  analogWrite(redLED, 0);
  analogWrite(grnLED, maxLumens);
  analogWrite(bluLED, maxLumens);
}
void turnLEDYlw(){
  analogWrite(redLED, maxLumens);
  analogWrite(grnLED, maxLumens);
  analogWrite(bluLED, 0);
}
void turnLEDMag(){
  analogWrite(redLED, maxLumens);
  analogWrite(grnLED, 0);
  analogWrite(bluLED, maxLumens);
}