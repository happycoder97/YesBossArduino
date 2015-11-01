#include <Arduino.h>
#include "Auth.h"
#include "EEPROM.h"
#include <string.h>
void Auth::storeToEEPROM() {
    int addr = EEPROM_START_ADDR;
    EEPROM.put(addr,EEPROM_SIGNATURE);
    addr += 1; //advance one byte
    EEPROM.put(addr,password);
    // addr+=PASSWORD_LEN;
}

bool Auth::restoreFromEEPROM() {
    byte addr = EEPROM_START_ADDR;
    byte signature;
    EEPROM.get(addr,signature);
    // fail if signature didn't match
    if(signature!=EEPROM_SIGNATURE) return false;
    addr+=1;
    EEPROM.get(addr,password);
    // addr+=PASSWORD_LEN;
    return true;
}

void Auth::resetAuth() {
    strcpy(password,DEFAULT_PASSWORD);
}

bool Auth::tryAuthenticate(char* password) {
    int i=0;
    while(password[i]!='\0') {
        if(this->password[i]!=password[i]) return false;
    }
    return true;
}

bool Auth::changePassword(char* old_password, char *new_password) {
    if(!tryAuthenticate(old_password)) return false;
    strcpy(password,new_password);
    return true;
}


