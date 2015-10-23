#include "Utils.h"
#include "DataParser.h"

void DataParser::parseRequest(char message[160], Request_Message& request_message) {
  #define bufferlen 10
  char buffer[bufferlen];
  byte messageCharIndex = 0;
  messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);

  //there was an error
  if(messageCharIndex<0) {
    request_message.message_type=REQ_INVALID;
    #ifdef DEBUG
    Serial.print("WARN: messageCharIndex=");
    Serial.println(messageCharIndex);
    #endif
    return;
  }

  if(buffer[0]!=MAGIC_CHAR) {
    request_message.message_type=REQ_INVALID;
    return;
    #ifdef DEBUG
    Serial.print("WARN: magic_char not found :");
    Serial.println(buffer[0]);
    #endif
  }
  //copy to request_message->blah->pwd @ index 0
  //from buffer start@index 1 (to avoid MAGIC_CHAR at beginning)
  strcpy(request_message.auth_data.password,0,buffer,1,-1);

  messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);

  if(streq(buffer,REQ_STATUS_TOKEN,0,0)) {
    request_message.message_type=REQ_STATUS;
    return;
  }
  if(streq(buffer,REQ_UPDATE_STATUS_TOKEN,0,0)) {
    request_message.message_type=REQ_UPDATE_STATUS;
    messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);

    bool mode;
    char numToChar[11]="0123456789";
    byte b; //some disposable counter
startenc:
    mode = streq(buffer,"ON",0,0);

    if(!mode&&!streq(buffer,"OFF",0,0)) {
      request_message.message_type=REQ_INVALID;
      #ifdef DEBUG
      Serial.print("WARN: invalid status found :");
      Serial.println(buffer);
      #endif
      return;
    }

    while(messageCharIndex>0) {
      messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);
      for(b=0;b<10;b++) {
        if(buffer[0]!=numToChar[b]) continue;
        byteSet(
          request_message.status_update_request.device_on_off,
          b,mode);
        byteSet(
          request_message.status_update_request.device_on_off_mask,
          b,true);
      }
      if(buffer[0]!=numToChar[b-1]) goto startenc;
    }
    return;
  }
  if(streq(buffer,REQ_CHANGE_PASSWORD_TOKEN,0,0)) {
    request_message.message_type=REQ_CHANGE_PASSWORD;
    messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);
    if(messageCharIndex<0) {
      request_message.message_type=REQ_INVALID;
      #ifdef DEBUG
      Serial.print("WARN: password extracting failed. status=");
      Serial.println(messageCharIndex);
      #endif
      return;
    }
    strcpy(request_message.change_password_request.new_password,
        0,buffer,0,-1);
    return;
  }
  if(streq(buffer,REQ_CHANGE_PHONE_TOKEN,0,0)) {
    request_message.message_type=REQ_CHANGE_PHONE;
    messageCharIndex=getNextWord(message,messageCharIndex,buffer,bufferlen);
    if(messageCharIndex<0) {
      request_message.message_type=REQ_INVALID;
      #ifdef DEBUG
      Serial.print("WARN: phone number extracting failed. status=");
      Serial.println(messageCharIndex);
      #endif
      return;
    }
    strcpy(request_message.change_phone_request.new_phone,
        0,buffer,0,-1);
    return;
  }
      
  request_message.message_type=REQ_INVALID;
  #ifdef DEBUG
  Serial.print("WARN: invalid token found :");
  Serial.println(buffer);
  #endif
  return;


}

void DataParser::parseStatusReply(char message[160],Status_Message_To_Phone& status) {
  int i = 0;
  message[i] = MAGIC_CHAR;
  i++;
  strcpy(message,i,"SUCCESS ",0,-1);
  i+=8;
  message[i]='\n';
  i++;
  strcpy(message,i,"ON ",0,-1);
  i+=3;
  byte j;
  for(j=0;j<8;j++) {
    if(status.device_on_off&(1<<j)) {
        i+=itoa(j,message,i,160);
        message[i-1]=' '; //to get rid of \0
    }
  }
  strcpy(message,i,"\nOFF ",0,-1);
  i+=5;
  for(j=0;j<8;j++) {
    if(!status.device_on_off&(1<<j)) {
        i+=itoa(j,message,i,160);
        message[i-1]=' '; //to get rid of \0
    }
  }
  strcpy(message,i,"\nWATERLEVEL ",0,-1);
  i+=12;
  i+=itoa(status.waterlevel_percent,message,i,160-i);
  message[i]='%';
  strcpy(message,i,"\nTEMP ",0,-1);
  i+=6;
  i+=itoa(status.temperature,message,i,160-i);
  message[i]='C';
  message[i+1]='\0';
}


