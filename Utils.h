
#include <Arduino.h>

void trimSpaces(char* string);
void strcpy(char* dest, int destbegin,  char* source,int sourcebegin, int length);

void byteSet(byte& b, byte n, bool val);

int getNextWord(char* stringToSplit,int start,char* buffer,int bufferlen);

bool streq(char* s1,char* s2,int s1begin,int s2begin);

int itoa(int i, char* s,int offset,int slen);
