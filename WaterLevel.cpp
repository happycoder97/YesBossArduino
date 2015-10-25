#include <Arduino.h>
#include "WaterLevel.h"

byte WaterLevel::getWaterLevel() {
  digitalWrite(pinOutput,HIGH);
  delay(100);
  if(digitalRead(pinHigh)==HIGH) {
		digitalWrite(pinOutput,LOW);
		return WATER_LEVEL_HIGH;
	}
  if(digitalRead(pinMedium)==HIGH) {
		digitalWrite(pinOutput,LOW);
		return WATER_LEVEL_MEDIUM;
	}
  if(digitalRead(pinLow)==HIGH) {
		digitalWrite(pinOutput,LOW);
		return WATER_LEVEL_LOW;
	}
	digitalWrite(pinOutput,LOW);
	return WATER_LEVEL_VERY_LOW;
}

WaterLevel::WaterLevel(
    byte pinLow, byte pinMedium, byte pinHigh, byte pinOutput) {
  this->pinLow = pinLow;
  this->pinMedium = pinMedium;
  this->pinHigh = pinHigh;
  this->pinOutput = pinOutput;
}

