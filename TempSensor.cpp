
#include "TempSensor.h"

TempSensor::TempSensor(byte dht11_pin) {
  this->dht11_pin=dht11_pin;
}

byte TempSensor::getTemp() {
  dht11.read(dht11_pin);
  return dht11.temperature;
}
