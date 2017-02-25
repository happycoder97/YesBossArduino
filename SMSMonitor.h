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
         * if hasSMS succeed.
         * Otherwise will blank the given array or previous sms is returned
         */
        void getSMS(char msg[160]);
        /*
         * Creates an instance of SMSMonitor
         * and instantiate the SerialGSM interface
         * rx_pin -> SIM900A TTL rx_pin
         * tx_pin -> SIM900A TTL tx_pin
         */
        SMSMonitor(SerialGSM &serial_gsm_ref);

    private:
        
        SerialGSM serial_gsm;
        SerialGSM::SMS sms_buffer;
        bool buffer_not_empty;

};

