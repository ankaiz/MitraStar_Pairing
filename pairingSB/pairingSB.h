#ifndef PAIRINGSB

#define PAIRINGSB

#include <stdio.h>          // for printf() and fprintf() 

#include <sys/socket.h>     // for socket(), bind(), and connect() 

#include <arpa/inet.h>      // for sockaddr_in and inet_ntoa() 

#include <stdlib.h>         // for atoi() and exit() 

#include <string.h>         // for memset() 

#include <unistd.h>         // for close() 

#include <fcntl.h>          // for fcntl() 

#include <errno.h>

#include <sys/epoll.h>

#include <ifaddrs.h>

#include <netinet/in.h>

#include <json-c/json.h>

#include <math.h>

#include <pthread.h>

#include <stdint.h>

#include <assert.h>

#include <openssl/buffer.h>

#include <pjlib.h>

#include <pjlib-util.h>

#include <pjnath.h>

#include <stdbool.h>

#include "SecurityModel.h"



#define MAX_SIZE 60001     

#define RECEIVEPORT 5556

#define SENDPORT 5555    

#define THIS_FILE   "icedemo.c"

#define KA_INTERVAL 300

  

struct sockaddr_in dstaddr; // This is the broadcast addr

struct sockaddr_in uniaddr; // This is the unicast addr

struct sockaddr_in srvaddr; // Broadcast Server Address

struct sockaddr_in cliaddr; // Broadcast Response Client Address



int sockfd;





int make_socket_non_blocking( int sockfd );

void checkPiaired(char *Key);

char *pGetSB_IP(void);

void  pSendBcast (void *ptr);

char *genKey(char *UUID);

bool saveKey(char *Key);

char *sendKey(char *url);

bool unpaired(char lanip[17]);

bool execCmd(char cmd[MAX_SIZE]);

bool sendPkg(int type , char udp[MAX_SIZE]);

bool pVerifyPW(char *pairing , char *pringingkey , char *publicip , char *publicport);

void pShowPW(void);

void json_parse(json_object * jobj,char *publicip , char *publicport);

bool saveKey(char *Key);

bool reset(void);



#endif


