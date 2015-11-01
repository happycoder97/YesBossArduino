#pragma once
#include<Arduino.h>

/*
 * Set according to your applications need
 */
#define EEPROM_START_ADDR 0

/*
 * This value verifies that it
 * is valid data starting at EEPROM_START_ADDR
 * Randomly change this value when
 * PASSWORD_LEN is updated
 * MUST BE less than 256 (ie one byte)
 */
 #define EEPROM_SIGNATURE 1
/*
 * change these values to suite your needs
 */
 #define PASSWORD_LEN 11

 #define DEFAULT_PASSWORD "KPCHSS"

/*
 * This class stores the password
 * of user and provides methods
 * to authenticate,change password and
 * reset password to a default
 * value.
 * Functions are provided to permanently
 * store the password
 * to EEPROM of the MCU.
 */

class Auth {
    public:
        /*
         * Save the password and mobile no
         * to EEPROM
         */
        void storeToEEPROM();
        /*
         * Restores the password and mobile no from
         * EEPROM to RAM
         * Must be called everytime the object
         * is constructed. Provided as a separate function
         * for lazy loading
         * returns false if the EEPROM
         * doesnot contain any valid data to restore
         */
        bool restoreFromEEPROM();
        /*
         * reset password to a default value
         */
        void resetAuth();
        /*
         * Simply password == this->password :P
         */
        bool tryAuthenticate(char password[PASSWORD_LEN]);
        /*
         * tryAuthenticate; if succeed:
         * this->password = newPassword
         * return true;
         * else return false
         */
        bool changePassword(char old_password[PASSWORD_LEN],char new_password[PASSWORD_LEN]);
    private:
        char password[PASSWORD_LEN];
};
