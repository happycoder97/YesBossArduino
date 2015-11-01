// SerialGSM version 1.1
// by Meir Michanie
// meirm@riunx.com


// error codes
// http://www.developershome.com/sms/resultCodes2.asp
#include "SerialGSM.h"

SerialGSM::SerialGSM(int rxpin,int txpin):
SoftwareSerial(rxpin,txpin) { }

void SerialGSM::fwdSMS2Serial(){
  #ifdef DEBUG
  Serial.println("AT+CMGF=1"); // set SMS mode to text
  #endif
  this->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  response = this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.print(buffer);
  Serial.println("<EOR>");
  Serial.println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
  #endif
  this->println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
  delay(200);
  response=this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
}
  /* returns:
   * 0 if succeed
   * 1 if length of the message>MAXMSGLEN
   * 2 for some other errors
   * TODO: return 0 only if OK was received
   */
byte SerialGSM::sendSMS(SMS& sms) {
  #ifdef DEBUG
  Serial.print("Sending SMS to :");
  Serial.println(sms.phone_no);
  Serial.print(sms.message);
  Serial.println("<EOF>");
  Serial.println("AT+CMGF=1"); // set SMS mode to text
  #endif

  this->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  response = this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");

  Serial.print("AT+CMGS=");
  Serial.print(char(34)); // ASCII equivalent of "
  Serial.print(sms.phone_no);
  Serial.println(char(34)); // ASCII equivalent of "
  #endif

  if(response<0) return 2;

  this->print("AT+CMGS=");
  this->print(char(34)); // ASCII equivalent of "
  this->print(sms.phone_no);
  this->println(char(34));  // ASCII equivalent of "
  delay(500); // give the module some thinking time

  response = this->readline(buffer,BUFFERLEN,length_read);

  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  Serial.print(sms.message);
  Serial.println("<EOF>")
  #endif

  if(response<0) return 2;

  this->print(sms.message);
  this->print(char(26));  // ASCII equivalent of Ctrl-Z
  delay(500);

  response = this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
  if(response<0) return 2;
  return 0;
}


void SerialGSM::deleteAllSMS(){
  #ifdef DEBUG
  Serial.println("AT+CMGD=1,4"); // delete all SMS
  #endif
  this->println("AT+CMGD=1,4"); // delete all SMS
  response = this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
}

void SerialGSM::reset(){
  #ifdef DEBUG
  Serial.println("AT+CFUN=1,1"); // Reset Modem
  #endif
  this->println("AT+CFUN=1,1"); // Reset Modem
  delay(200);
  response = this->readline(buffer,BUFFERLEN,length_read);
  #ifdef DEBUG
  Serial.print("Read length:");
  Serial.println(length_read);
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
}

/*
 *  0 if everything alright
 * -1 if buffer overflow
 * -2 if timeout
 * -3 end of input before <LF>
 */
int SerialGSM::readline(char buffer[],int bufferlen, int& length_read){
  length_read = 0;
  char nc;
  while (this->available()){
    nc=this->read();
    if(length_read>bufferlen) { return -1; }
    if(nc=='\r') continue; //skip CR
    if(nc=='\n') {
      lastrec = millis();
      buffer[length_read]='\0';
      return length_read;
    }
    if((millis()> lastrec + SERIALTIMEOUT) && (length_read > 0)) {
      return -2;
    }
    buffer[length_read]=nc;
    lastrec=millis();
    length_read++;
  }
  return -3;
}

/*
 * returns:
 * >
 * 3 if empty message
 * (-1,-2,-3)+3 errors caused by readline
 * 4 sms buffer overflow
 * 5 phone no buffer overflow
 */
int SerialGSM::receiveSMS(SMS& sms) {
  // response = readline(buffer,BUFFERLEN,length_read);
  // #ifdef DEBUG
  // Serial.println("Trying to read sms from serial..");
  // Serial.print("Read length:");
  // Serial.println(length_read);
  // Serial.print("Response:");
  // Serial.println(response);
  // Serial.println(buffer);
  // Serial.println("<EOR>");
  // #endif
  // if(response<=0) return response;
  // if (this->readline(buffer,BUFFERLEN,length_read)>5){
  // // Get the number of the sms sender in order to be able to reply
	// if ( strstr(response, "CMT: ") != NULL ){
	//     int sf=6;
	//     if(strstr(response, "+CMT:")) sf++;
	// 	    for (int i=0;i < PHONESIZE;i++){
	// 	      sendernumber[i]=inmessage[sf+i];
	// 	    }
// 		sendernumber[PHONESIZE]='\0';
// 		return 0;
// 	 }else{
// 		if(insms) {
// 			insms=0;
// 			return 1;
// 		}
// 	}
//   }
  return 0;
}


void SerialGSM::boot(){
  #ifdef DEBUG
  Serial.print("Waiting for modem to turn on.");
  #endif
  int counter=0;
  while(counter++ < 15){
    #ifdef DEBUG
    Serial.print(".");
    #endif
    delay(1000);
  }
  #ifdef DEBUG
  Serial.println();
  Serial.println("Now forwarding SMS to Serial");
  #endif
  fwdSMS2Serial();
}

