#include "Utils.h"

void strcpy(char* dest, int destbegin,  const char* source,int sourcebegin, int length) {
    int i=0;
    while(source[i+sourcebegin]!='\0'&&(length!=-1||i<length)) {
        if(length!=-1&&i>=length) return;
        dest[i+destbegin] = source[i+sourcebegin];
        i++;
    }
    dest[i+destbegin]='\0';
}

void trimSpaces(char* string) {
    if(string[0]==' ') {
        byte i = 0;
        while(string[i]!='\0') {
            string[i] = string[i+1];
            i++;
        }
        if(string[i-1]==' ')
            string[i-1] = '\0';
    } else {
        byte i = 0;
        while(string[i]!='\0') i++;
        if(string[i-1]==' ')
            string[i-1] = '\0';
    }
}

void byteSet(byte& b, byte n, bool val) {
    if(val) b|=(1<<n);
    else b&=~(1<<n);
}

/*
 * finds the next word in stringToSplit starting from start
 * and stores it in buffer
 * returns the index of last char in a word + 1
 * returns -1 if no words are found
 * returns -2, if buffer is not large enough to hold that word
 */
int getNextWord(char* stringToSplit,int start,char* buffer,int bufferlen) {
    byte i=start;
    //go forward until start of next word
    //which is identified by a character other
    //than space CR or LF
    while(stringToSplit[i]!=' '
        ||stringToSplit[i]!='\n'
        ||stringToSplit[i]!='\r'
        ||stringToSplit[i]!='\0'
        ) i++;

    //if a null char is found before a word
    //set buffer to null char and return the index
    if(stringToSplit[i]=='\0') {
        buffer[0]='\0';
        return -1;
    }
    byte j = 0;
    while(stringToSplit[i]!=' '
        &&stringToSplit[i]!='\n'
        &&stringToSplit[i]!='\r'
        &&stringToSplit[i]!='\0') {
            //if there is not enough space for
            //this char + null char, return -2
            if(j+1>=bufferlen) return -2;
            buffer[j] = stringToSplit[i];
            i++;j++;
    }
    //append nullchar
    buffer[j] = '\0';
    // return the index of char after
    // the end of this word
    return i;
}
/* str1 == str2 */
bool streq(const char* s1,const char* s2,int s1begin,int s2begin) {
    int i = 0;
    while(s1[i+s1begin]==s2[i+s2begin]) {
        if(s1[i+s1begin]=='\0') return true;
        i++;
    }
    return false;
}

int itoa(int i, char* s,int offset,int slen) {
  char itoa[11] = "0123456789";
  int j=0;
  while(i>0) {
    if(j+1>=slen) return -1;
    s[offset+j] = itoa[i%10];
    i/=10;
    j++;
  }
  s[offset+j]='\0';
  return offset+j;
}

/*
 * return the length of the string
 * EXCLUDING null character
 */
int strlen(char* s) {
  int len=0;
  while(s[len]!='\0') len++;
  return len;
}
