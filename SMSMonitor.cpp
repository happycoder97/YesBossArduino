
#include<Arduino.h>
#include"SMSMonitor.h"

SMSMonitor::SMSMonitor(SerialGSM &serial_gsm_ref) {
    serial_gsm = serial_gsm_ref;
}

bool SMSMonitor::hasSMS() {
    // Copies msg to buffer and return true if error code is 0;
    return serial_gsm.receiveSMS(sms_buffer) == 0;
}

void getSMS(char msg[160]) {
    // Caution: Hard coded max sms length: 160
    // Copy the msg to given array
    strcpy(msg,0,sms_buffer.message,0,160);
}

