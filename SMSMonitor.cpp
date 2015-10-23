
#include<Arduino.h>
#include<SMSMonitor.h>

SMSMonitor::SMSMonitor(byte rx_pin,byte tx_pin) {
    serial_gsm(rx_pin,tx_pin);
    serial_gsm.Verbose(false);

