#include <iostream>
#include "../Utils.h"
using namespace std;

void testStreq() {
  cout<<"Testing streq(char*,char*):bool ..";
  char a[] = "TEST CHAR \n\r ";
  if(streq(a,a,0,0)) cout<<"Success!";
  else cout<<"Fail!";
  cout << endl;
  cout<<"Testing streq(char*,char*,start1,start2):bool ..";
  char b[] = "  TEST CHAR \n\r ";
  if(streq(a,b,0,2)) cout<<"Success!";
  else cout<<"Fail!";
  cout << endl;
}
void testTrimSpaces() {
  char input[]=" asd ";
  char out[]="asd";
  cout<<"Testing trimspaces.. ";
  trimSpaces(input);
  cout << input << endl;
  if(streq(input,out,0,0)) cout<<"Success!";
  else cout<<"Fail!";
  cout << endl;
}
void testStrIndexof() {
  cout<<"Testing indexof.."<<endl;
  int i=0;
  char hay[] = "abc abc abc abc def abc  ";
  char needle[] = "abc";
  i=str_indexof(hay,needle,0);
  if(i==0) cout<<"Test 1 succeed!"<<endl;
  i=str_indexof(hay,needle,1);
  if(i==4) cout<<"Test 2 succeed!"<<endl;
  i=str_indexof(hay,needle,4);
  if(i==4) cout<<"Test 3 succeed!"<<endl;
  i=str_indexof(hay,needle,15);
  if(i==20) cout<<"Test 4 succeed!"<<endl;
  i=str_indexof(hay,needle,21);
  if(i==-1) cout<<"Test 5 succeed!"<<endl;
  cout<<"DONE!";
}
int main() {
  testStreq();
  testTrimSpaces();
  testStrIndexof();
  return 0;
}

