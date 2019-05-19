#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include"charbinary.h"
#include<time.h>
#include<string.h>
#include<limits.h>
#include<sys/time.h>
#define MAX_STRING 1000

int getOtherPID();
void signalHandler(int sigNum);
int receiverCounter = 7;
int receiver[8];
long int diff = 0;
int tracker = 0;
int MESSAGE_RECEIVED[MAX_STRING + 2];
int characterCount= 0;
int error = 0;
int headerError = 0;
int headerReceived = -1;
struct timeval last, current;

int main(){
    printf("Own PID: %d \n", getpid());
    int otherPID = getOtherPID();

    struct sigaction sigActStruct;
  	memset (&sigActStruct, '\0', sizeof(sigActStruct));
  	sigActStruct.sa_handler = &signalHandler;
  	sigActStruct.sa_flags = SA_RESTART;
  	sigaction(SIGUSR1, &sigActStruct, NULL);
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 6000000;
    char message[MAX_STRING];

    #ifdef SINGLE
    while(1){
       fgets(message,MAX_STRING, stdin);
       if ((message[0] == '.') & (message[1] == '\n')){
           exit(1);
       }
       int index = 0;
       int header = 0;
       while (message[index] != 0){
           header += message[index++] ;
       }
       header = header % 256;
       int *headerRepr = charToBinary(header);
       for (int bit = 7; bit >= 0; bit--){
           if (headerRepr[bit] == 0){
               kill(otherPID, SIGUSR1);
               nanosleep(&tim , &tim2);
               kill(otherPID, SIGUSR1);
           }
           else{
               tim.tv_nsec = 1500000;
               kill(otherPID, SIGUSR1);
               nanosleep(&tim , &tim2);
               kill(otherPID, SIGUSR1);
               tim.tv_nsec = 6000000;
           }
           nanosleep(&tim , &tim2);
           // tim2 gets remaining time if nanosleep is interrupted
       }

       index = 0;
       while(message[index] != 0 ){
           int *binaryRepr = charToBinary(message[index]);
           for (int bit = 7; bit >= 0; bit--){
               if (binaryRepr[bit] == 0){
                   kill(otherPID, SIGUSR1);
                   nanosleep(&tim , &tim2);
                   kill(otherPID, SIGUSR1);
               }
               else{

                   tim.tv_nsec = 1500000;
                   kill(otherPID, SIGUSR1);
                   nanosleep(&tim , &tim2);
                   kill(otherPID, SIGUSR1);
                   tim.tv_nsec = 6000000;
               }
               nanosleep(&tim , &tim2);
               // tim2 gets remaining time if nanosleep is interrupted
           }
           index += 1;
       }
    }
    #endif

    #ifndef SINGLE
    sigaction(SIGUSR2, &sigActStruct, NULL);
    while(1){
      fgets(message,MAX_STRING, stdin);
      if ((message[0] == '.') & (message[1] == '\n')){
          exit(1);
      }
      int index = 0;
      int header = 0;
      while (message[index] != 0){
          header += message[index++] ;
      }
      header = header % 256;
      int *headerRepr = charToBinary(header);
      for (int bit = 7; bit >= 0; bit--){
          if (headerRepr[bit] == 0){
              kill(otherPID, SIGUSR1);
          }
          else{
            kill(otherPID, SIGUSR2);
          }
          nanosleep(&tim , &tim2);
          // tim2 gets remaining time if nanosleep is interrupted
      }

      index = 0;
      while(message[index] != 0){
          int *binaryRepr = charToBinary(message[index]);
          for (int bit = 7; bit >= 0; bit--){
              if (binaryRepr[bit] == 0){
                  kill(otherPID, SIGUSR1);
              }
              else{
                  kill(otherPID, SIGUSR2);
              }
              nanosleep(&tim , &tim2);
          }
          index += 1;
      }
    }
    #endif
    return 0;
}

int getOtherPID() {
    char response[MAX_INPUT];
    fgets(response, MAX_INPUT, stdin);
    int otherPID;
    sscanf(response, "%d", &otherPID);
    return otherPID;
}

// sigNum is SIGUSR1 or 2
#ifndef SINGLE
void signalHandler(int sigNum){

    if (sigNum == SIGUSR1){
        receiver[receiverCounter--] = 0;
    }
    else if (sigNum == SIGUSR2){
        receiver[receiverCounter--] = 1;
    }

    if (receiverCounter == -1){
        int temp = binaryToChar(receiver);
        if ((temp == 10) & (headerReceived != -1)) {
          headerError = (headerError + temp) % 256;
          printf("\n");
          if ((error == 0) & (headerReceived == headerError))printf("! ");
          else printf("? ");
          for (int i = 0; i < characterCount; i++) {
            printf("%c", MESSAGE_RECEIVED[i]);
          }
          printf("\n");
          characterCount = 0;
          error = 0;
          headerReceived = -1;
          headerError = 0;
        }
        else {
          if ((characterCount == 0) & (headerReceived == -1)) headerReceived = temp;
          else {
            if (( temp > 127) || (temp < 32)) error = 1;
            MESSAGE_RECEIVED[characterCount++] = temp;
            headerError = (headerError + temp) % 256;
          }
        }
        receiverCounter = 7;
    }
    return;
}
#endif

#ifdef SINGLE
void signalHandler(int sigNum){
    last = current;
    gettimeofday(&current, NULL);
    diff = ((current.tv_sec * 1000000 + current.tv_usec) - (last.tv_sec * 1000000 + last.tv_usec));

    if (tracker == 0){
        tracker = 1;
        }
    else{
        if (diff < 6000){
            receiver[receiverCounter] = 1;
        }
        else{
            receiver[receiverCounter] = 0;
        }
        tracker = 0;
        receiverCounter -= 1;
    }

    if (receiverCounter == -1){
        int temp = binaryToChar(receiver);
        if ((temp == 10) & (headerReceived != -1)) {
          headerError = (headerError + temp) % 256;
          printf("\n");
          if ((error == 0) & (headerReceived == headerError))printf("! ");
          else printf("? ");
          for (int i = 0; i < characterCount; i++) {
            printf("%c", MESSAGE_RECEIVED[i]);
          }
          printf("\n");
          characterCount = 0;
          error = 0;
          headerReceived = -1;
          headerError = 0;
        }
        else {
          if ((characterCount == 0) & (headerReceived == -1)) headerReceived = temp;
          else {
            if (( temp > 126) || (temp < 32)) error = 1;
            MESSAGE_RECEIVED[characterCount++] = temp;
            headerError = (headerError + temp) % 256;
          }
        }
        receiverCounter = 7;
    }
    return;
}
#endif
