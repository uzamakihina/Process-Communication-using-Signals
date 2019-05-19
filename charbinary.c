#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>
#include"charbinary.h"


int array[8];
int* charToBinary(int character) {
   for (int i = 7 ; i >= 0 ; i-- ) {
     array[i] = ((character & (int)pow(2,i)) >> i) > 0;
   }
   return array;
}

int binaryToChar(int array[]) {
  int character = 0;
  for (int i = 7 ; i >= 0 ; i-- ) {
    if (array[i] == 1) {
      character += pow(2,i);
    }
  }
  return character;
}
