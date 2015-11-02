// #define byte unsigned char
#include <Arduino.h>

void trimSpaces(char* string);
void strcpy(char* dest, int destbegin, const char* source,int sourcebegin, int length);

void byteSet(byte& b, byte n, bool val);

int getNextWord(char* stringToSplit,int start,char* buffer,int bufferlen);

bool streq(const char* s1,const char* s2,int s1begin,int s2begin);

int itoa(int i, char* s,int offset,int slen);

int strlen(char* s);

int str_indexof(char hay[],const char needle[],int i);
