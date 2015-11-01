#include <Arduino.h>
#include "SRRelayController.h"


SRRelayController::SRRelayController(byte sr_cp,byte sr_ds,byte sr_latch) {
    pin_sr_cp = sr_cp;
    pin_sr_ds = sr_ds;
    pin_sr_latch = sr_latch;
}

void SRRelayController::setDeviceStatus(byte device_status) {
    this->device_status = device_status;
}

void SRRelayController::flush() {
    digitalWrite(pin_sr_latch,LOW);
    shiftOut(pin_sr_ds, pin_sr_cp,MSBFIRST, device_status);
    digitalWrite(pin_sr_latch, HIGH);
    delay(10);
    digitalWrite(pin_sr_latch, LOW);
}
