#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "GlobalOptions.h"
#include "Utils.h"
#include "SerialGSM.h"
#include "SRRelayController.h"
#include "WaterLevel.h"
#include "SMSMonitor.h"
#include "TempSensor.h"
#include "DataParser.h"
#include "Auth.h"
#include "MemoryFree.h"

SerialGSM serial_gsm(GSM_RX,GSM_TX);
SRRelayController relay_controller(RELAY_SR_CP,RELAY_SR_DS,RELAY_SR_LATCH);
WaterLevel water_level(WATER_LEVEL_1,WATER_LEVEL_2,WATER_LEVEL_3,WATER_LEVEL_0);
TempSensor temp_sensor(TEMP_SENSOR_DATA);
DataParser dataParser;
Auth auth;

SerialGSM::SMS sms;
void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Debugging activated..");
  Serial.println("---------------------");
  #endif
  serial_gsm.begin(9600);
  // pinMode(GSM_RX,INPUT);
  // pinMode(GSM_TX,OUTPUT);
  pinMode(RELAY_SR_CP,OUTPUT);
  pinMode(RELAY_SR_DS,OUTPUT);
  pinMode(RELAY_SR_LATCH,OUTPUT);
  pinMode(WATER_LEVEL_1,OUTPUT);
  pinMode(WATER_LEVEL_2,OUTPUT);
  pinMode(WATER_LEVEL_3,OUTPUT);
  pinMode(WATER_LEVEL_0,INPUT);
  pinMode(TEMP_SENSOR_DATA,INPUT);

  if(!auth.restoreFromEEPROM()) auth.resetAuth();
}


void sendStatusReplyToPhone() {
  DataParser::Status_Message_To_Phone status;
  status.device_on_off=relay_controller.getDeviceStatus();
  status.temperature=temp_sensor.getTemp();
  status.waterlevel_percent=water_level.getWaterLevel();

  dataParser.parseStatusReply(sms.message,status);
  strcpy(sms.phone_no,auth.getPhone());
  serial_gsm.sendSMS(sms);
}



void loop() {
  /* while(true) serial_gsm.debug_loop(); */
  if(Serial.available()) {
    if(Serial.read()=='a') {
      sendStatusReplyToPhone();
      Serial.println(freeMemory());
    }
  }
}

