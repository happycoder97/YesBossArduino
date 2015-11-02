#include<Arduino.h>
#include "GlobalOptions.h"

#define WATER_LEVEL_VERY_LOW 20
#define WATER_LEVEL_LOW 30
#define WATER_LEVEL_MEDIUM 60
#define WATER_LEVEL_HIGH 90

class WaterLevel {
  public:
    WaterLevel(
        byte pinLow,byte pinMedium, byte pinHigh,
        byte pinOutput);
    byte getWaterLevel();
  private:
    byte pinLow;
    byte pinMedium;
    byte pinHigh;
    byte pinOutput;
};
