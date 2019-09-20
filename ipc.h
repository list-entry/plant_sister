#ifndef __IPC_H
#define __IPC_H
#include "config.h"
struct msg_st{
	long int msgtype;
	int index;
};
int snd_req(void);
int rcv_req(void);
struct humi_ture * get_shm(void);
int put_shm(void);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int open_port(char *com);
void usart_get(char *c);
#endif
