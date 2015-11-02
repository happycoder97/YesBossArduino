#include<Arduino.h>

/*
 * updating this value doesnot
 * have any effect as of now.
 * currently, this stays just as an
 * info
 */
#define MAX_NO_OF_DEVICES 8

/*
 * Control multiple relays
 * (directly or indirectly via ICs)
 * with few digital pins, with the
 * help of a latched shift register (eg. 74HC595)
 */
class SRRelayController {
    public:
        /*
         * Instantiate SRRelayController
         * Accepts digital pins shift register is
         * connected to
         * Caution: all of these pins must be configured
         * to be in the output mode.
         */
        SRRelayController(byte sr_cp,byte sr_ds,byte sr_latch);
        /*
         * Update device_status
         * LSB -> SR first output
         * Note: this will only update the in memory
         * status. Call flush() to reflect this change
         * in hardware
         */
        void setDeviceStatus(byte device_status);
        /*
         * get device status
         */
        byte getDeviceStatus();
        /*
         * Since updating hardware takes time
         * hardware is not immediately updated on
         * calling turnDeviceOnOff function
         * this also allows to push in a bunch of
         * changes at once.
         */
        void flush();

    private:
        byte pin_sr_cp;
        byte pin_sr_ds;
        byte pin_sr_latch;
        /*
         * Currently stores device_status in bytes
         * can be turned to int or long in future
         */
        byte device_status = 0b00000000;
};
