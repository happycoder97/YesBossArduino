#include <Arduino.h>
#include <EEPROM.h>
#include "Auth.h"
#include "Utils.h"
void Auth::storeToEEPROM() {
    int addr = EEPROM_START_ADDR;
    EEPROM.put(addr,EEPROM_SIGNATURE);
    addr += 1; //advance one byte
    EEPROM.put(addr,password);
    addr+=sizeof(password);
    EEPROM.put(addr,phone);
}

bool Auth::restoreFromEEPROM() {
    byte addr = EEPROM_START_ADDR;
    byte signature;
    EEPROM.get(addr,signature);
    // fail if signature didn't match
    if(signature!=EEPROM_SIGNATURE) return false;
    addr+=1;
    EEPROM.get(addr,password);
    addr+=sizeof(password);
    EEPROM.get(addr,phone);
    return true;
}

void Auth::resetAuth() {
    strcpy(password,DEFAULT_PASSWORD);
    strcpy(phone,DEFAULT_PHONE);
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

char* Auth::getPhone() {
  return phone;
}

void Auth::setPhone(char* phone) {
  strcpy(this->phone,phone);
}
