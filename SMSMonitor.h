#include <Arduino.h>
#include "SerialGSM.h"

class SMSMonitor {

    public:
        /*
         * Checks GSM modem for any sms
         */
        bool hasSMS();
        /*
         * Get next sms
         */
        bool nextSMS(char msg[160]);
        /*
         * Creates an instanc of SMSMonitor
         * and instantiate the SerialGSM interface
         * rx_pin -> SIM900A TTL rx_pin
         * tx_pin -> SIM900A TTL tx_pin
         */
        SMSMonitor(byte rx_pin,byte tx_pin);

    private:
        
        SerialGSM serial_gsm;
        SerialGSM::SMS sms_buffer;
        bool buffer_not_empty;

};

