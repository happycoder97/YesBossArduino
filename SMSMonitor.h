#include <Arduino.h>
#include <

class SMSMonitor {

    public:
        /*
         * Checks GSM modem for any sms
         */
        bool hasSMS();
        /*
         * Get next sms
         */
        char[160] nextSMS();
        /*
         * Creates an instanc of SMSMonitor
         * and instantiate the SerialGSM interface
         * rx_pin -> SIM900A TTL rx_pin
         * tx_pin -> SIM900A TTL tx_pin
         */
        SMSMonitor(byte rx_pin,byte tx_pin);

    private:
        //Deleted for saving memory
        //byte rx_pin;
        //byte tx_pin;
        SerialGSM serial_gsm;

};

