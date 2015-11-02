#include<Arduino.h>
#include "GlobalOptions.h"
#include "DHT11.h"

class TempSensor {
  public:
    TempSensor(byte dht11_pin);
    byte getTemp();
  private:
    DHT11 dht11;
    byte dht11_pin;

};

