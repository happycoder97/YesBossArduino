// SerialGSM version 1.1
// by Meir Michanie
// meirm@riunx.com


// error codes
// http://www.developershome.com/sms/resultCodes2.asp
#include <SoftwareSerial.h>
#include "SerialGSM.h"
#include "Utils.h"

SerialGSM::SerialGSM(int rxpin,int txpin): SoftwareSerial(rxpin,txpin) { 
}

void SerialGSM::fwdSMS2Serial(){
  println_d("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  response = readline(buffer,BUFFERLEN,length_read);
  println_d("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
  delay(200);
  response=readline(buffer,BUFFERLEN,length_read);
}
  /* returns:
   * 0 if succeed
   * 1 if length of the message>MAXMSGLEN
   * 2 for some other errors
   * TODO: return 0 only if OK was received
   */
byte SerialGSM::sendSMS(SMS& sms) {
  #ifdef DEBUG
  Serial.print(":: Sending SMS to :");
  Serial.println(sms.phone_no);
  Serial.print(sms.message);
  Serial.println("<EOF>");
  #endif

  println_d("AT+CMGF=1"); // set SMS mode to text
  delay(100);
  response = readline(buffer,BUFFERLEN,length_read);
  if(!isListening()) Serial.println(":: not listening!");
  if(response<0) return 2;

  print_d("AT+CMGS=\"");
  print_d(sms.phone_no);
  println_d('"');  // ASCII equivalent of "
  delay(500); // give the module some thinking time

  response = readline(buffer,BUFFERLEN,length_read);
  if(response<0) return 2;

  println_d(sms.message);
  print_d(char(26));  // ASCII equivalent of Ctrl-Z
  delay(500);

  response = readline(buffer,BUFFERLEN,length_read);
  if(response<0) return 2;
  #ifdef DEBUG
  Serial.println(":: Message sending succeded.");
  #endif
  return 0;
}


void SerialGSM::deleteAllSMS(){
  println_d("AT+CMGD=1,4"); // delete all SMS
  response = readline(buffer,BUFFERLEN,length_read);
}

void SerialGSM::reset(){
  println_d("AT+CFUN=1,1"); // Reset Modem
  delay(200);
  response = readline(buffer,BUFFERLEN,length_read);
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
    /* The below lines are added to investigate why
     * length_read was reported to be > 0
     * but Serial.print(buffer) was empty
     */
    Serial.print(nc);
    if(nc==0) Serial.println("OOPS!");
    else Serial.println("NOOPS!");
    if(length_read>bufferlen) {
      #ifdef DEBUG
      Serial.print(":: readline (-1): BUFFER OVERFLOW read ");
      Serial.print(length_read);
      Serial.print(" chars");
      Serial.print(":<- ");
      Serial.print(buffer);
      Serial.println("<EOR>");
      #endif
      return -1;
    }
    if(nc=='\r') continue; //skip CR
    if(nc=='\n') {
      lastrec = millis();
      buffer[length_read]='\0';
      #ifdef DEBUG
      Serial.print("- readline (0): read ");
      Serial.print(length_read);
      Serial.print(" chars");
      Serial.print(":<- ");
      Serial.print(buffer);
      Serial.println("<EOR>");
      #endif
      return 0;
    }
    if((millis()> lastrec + SERIALTIMEOUT) && (length_read > 0)) {
      #ifdef DEBUG
      Serial.print("- readline (-2): TIMEDOUT read ");
      Serial.print(length_read);
      Serial.print(" chars");
      Serial.print(":<- ");
      Serial.print(buffer);
      Serial.println("<EOR>");
      #endif
      return -2;
    }
    
    buffer[length_read]=nc;
    lastrec=millis();
    length_read++;
  }
  #ifdef DEBUG
  Serial.print("- readline (-3): Serial.available() = false; read ");
  Serial.print(length_read);
  Serial.print(" chars");
  Serial.print(":<- ");
  Serial.print(buffer);
  Serial.println("<EOR>");
  #endif
  return -3;
}

/*
 * returns:
 * -1 no message
 * -2 sms buffer overflow
 */
int SerialGSM::receiveSMS(SMS& sms) {
  response = readline(buffer,BUFFERLEN,length_read);
  if(response<0) return -1;

  if (readline(buffer,BUFFERLEN,length_read)<7) return -1;
  //if CMT: is not in the line, it is not a message
	if (str_indexof(buffer, "CMT: ",0) ==-1 ) return -1;

  byte ph_start=str_indexof(buffer,"\"",0);
  byte ph_end = str_indexof(buffer,"\"",ph_start+1);
  // "PH_NO" | ph_start = 0 | ph_end = 6
  // length should be 5 | it is (ph_end - ph_start) - 1
  strcpy(sms.phone_no,0,buffer,ph_start+1,(ph_end-ph_start)-1);
  int sms_i=0;
  do {
    response = readline(buffer,BUFFERLEN,length_read);
    if(sms_i+length_read>MAXMSGLEN) {
      #ifdef DEBUG
      Serial.println(":: ERROR recieveSMS sms buffer overflow.");
      #endif
      return -2;
    }
    strcpy(sms.message,sms_i,buffer,0,length_read);
    sms_i+=length_read;
  } while(response>=0);
  return 0;
}


void SerialGSM::boot(){
  #ifdef DEBUG
  Serial.print(":: Waiting for modem to turn on.");
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
  Serial.println(":: Now forwarding SMS to Serial");
  #endif
  fwdSMS2Serial();
}
// ------------- DEBUG LOGGING WRAPPERS ------------
// The following functions wrap around print and println functions
// to show DEBUG LOG if DEBUG is defined
void SerialGSM::print_d(const char* s) {
 #ifdef DEBUG
 if(debug_show_prompt) Serial.print(":-> ");
 Serial.print(s);
 debug_show_prompt= false;
 #endif
 print(s);
}

void SerialGSM::println_d(const char* s) {
 #ifdef DEBUG
 if(debug_show_prompt) Serial.print(":-> ");
 Serial.println(s);
 debug_show_prompt= true;
 #endif
 println(s);
}
void SerialGSM::print_d(const char s) {
 #ifdef DEBUG
 if(debug_show_prompt) Serial.print(":-> ");
 Serial.print(s);
 debug_show_prompt= false;
 #endif
 print(s);
}

void SerialGSM::println_d(const char s) {
 #ifdef DEBUG
 if(debug_show_prompt) Serial.print(":-> ");
 Serial.println(s);
 debug_show_prompt= true;
 #endif
 println(s);
}

void SerialGSM::debug_loop() {
  if(Serial.available()) print(Serial.read());
  if(available()) Serial.print(read());
}
