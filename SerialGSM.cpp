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
  Serial.println("AT+CMGF=0"); // set SMS mode to text
  #endif
  this->println("AT+CMGF=0"); // set SMS mode to text
  delay(200);
  response = this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
  Serial.print("Response:");
  Serial.println(response);
  Serial.print(buffer);
  Serial.println("<EOR>");
  Serial.println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
  #endif
  this->println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
  delay(200);
  response=this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
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
  Serial.println("AT+CMGF=0"); // set SMS mode to text
  #endif

  this->println("AT+CMGF=0"); // set SMS mode to text
  delay(200);
  response = this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
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

  response = this->readline(buffer,BUFFERLEN);

  #ifdef DEBUG
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

  response = this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
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
  response = this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
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
  response = this->readline(buffer,BUFFERLEN);
  #ifdef DEBUG
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
}

/*
 * returns length of string read
 * -1 if buffer overflow
 * -2 if timeout
 * -3 unexpected end of input
 */
int SerialGSM::readline(char* buffer,int bufferlen){
  int pos=0;
  char nc;
  while (this->available()){
    nc=this->read();
    if(pos>bufferlen) { return -1; }
    if(nc=='\r') {
      lastrec = millis();
      buffer[pos]='\0';
      return pos;
    }
    if((millis()> lastrec + SERIALTIMEOUT) && (pos > 0)) {
      return -2;
    }
    buffer[pos]=nc;
    lastrec=millis();
    pos++;
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
  response = readline(buffer,BUFFERLEN);
  #ifdef DEBUG
  Serial.println("Trying to read sms from serial..");
  Serial.print("Response:");
  Serial.println(response);
  Serial.println(buffer);
  Serial.println("<EOR>");
  #endif
  if(response<=0) return response;
  if (this->readline(buffer,BUFFERLEN)>5){
  // Get the number of the sms sender in order to be able to reply
	if ( strstr(inmessage, "CMT: ") != NULL ){
	    int sf=6;
	    if(strstr(inmessage, "+CMT:")) sf++;
		    for (int i=0;i < PHONESIZE;i++){
		      sendernumber[i]=inmessage[sf+i];
		    }
		sendernumber[PHONESIZE]='\0';
		return 0;
	 }else{
		if(insms) {
			insms=0;
			return 1;
		}
	}
  }
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
/*
 * store the GSM7 representation of 'in'
 * in 'gsm7'
 * In our case, only '$' matters
 */
void char_to_GSM7(char in, char& gsm7) {
  /* every char we use, except the '$'
   * has same representation in ASCII and GSM7 */
  if(in=='$') gsm7=2;
  else gsm7=in;
}

/*
 * store the hexadecimal representation of decimal in char *c
 * c must be at least of length startIndex+3
 */
void byte2hex_2char(byte dec,unsigned char *c,int startIndex) {
  char hexTable[17] = "0123456789ABCDEF";
  c[startIndex+0]=hexTable[dec/16];
  c[startIndex+1]=hexTable[dec%16];
  c[startIndex+2]='\0';
}
/*
 * convert ascii sms message content
 * to pdu format to out[] starting from out_start
 */
void user_payload_to_pdu(unsigned char in[],unsigned char out[],int out_start) {
  int in_i = 0;
  int out_i = 0;
  byte space_left_in_oct = 8;

  while(in[in_i]!='\0') {
    //the current char is not NUL char
    if(space_left_in_oct==8) { //all the octet is free
      out[out_i+out_start] = in[in_i];
      in_i++;
      space_left_in_oct = 1;
    } else{
      //if the octet is not completely empty
      //append current char's 'space_left_in_oct' bits
      //to the out octet
      out[out_i+out_start] &= 0b11111111>>space_left_in_oct;
      out[out_i+out_start] |= in[in_i]<<(8-space_left_in_oct);
      out_i++;
      out[out_i+out_start] = in[in_i]>>space_left_in_oct;
      space_left_in_oct++;
      in_i++;

    }
  }
  out[out_i+out_start+1]='\0';
}

/*
 * Convert pdu content to ascii message
 * given a string 'in', starting from pdu_start'th character
 * upto pdu_len characters
 */
void pdu_to_user_payload(unsigned char in[],int pdu_start,int pdu_len,unsigned char out[]) {
  int i = 0;
  int out_i = 0;
  int left_over_from_in=8;
  while(i<pdu_len) {
    if(left_over_from_in==8) {
      out[out_i] = in[i+pdu_start];
      out[out_i] &= 0b01111111;
      out_i++;
      left_over_from_in = 1;
    } else {
      out[out_i] = in[i+pdu_start]>>(8-left_over_from_in);
      i++;
      out[out_i] |= in[i+pdu_start]<<left_over_from_in;
      out[out_i] &= 0b01111111;
      left_over_from_in++;
      out_i++;
    }
  }
  out[out_i+1]='\0';
}
