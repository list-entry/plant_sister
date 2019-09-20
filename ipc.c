#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
//extern char  str[5];
int snd_req(void)
{
	key_t key=0x1234;
	int qid;
	struct msg_st msg;
	msg.msgtype = 1;
	msg.index = 1;
	qid = msgget(key,0666|IPC_CREAT);
	if(qid<0)
	{
		perror("msgget error\n");
		return 1;
	}
	if(msgsnd(qid,&msg,1,0)<0)
	{
		perror("msgsnd error\n");
		return 1;
	}
	return 0;

}

int rcv_req(void)
{
	key_t key=0x1234;
	int qid;
	struct msg_st msg;
	int shmrt;
	qid = msgget(key,0666|IPC_CREAT);
	if(qid<0)
	{
	//	perror("msgget error\n");
	//	return 1;
	}
	
	while(1){
		if(msgrcv(qid,&msg,4,1,0)<0)
		{
		//	perror("msgrcv error\n");
		//	return 1;
		}
		
		//if(msg.index==1)
		{
			shmrt=put_shm();
			if(shmrt!=0)
			{
				perror("put_shm error\n");
				exit(1);
			}
			sleep(2);
		}


	}
	return 0;

}

int put_shm(void)
{
	int shmid;
	key_t key = 0x2222;
	//static float i= 55,j=18;
	struct humi_ture* ht= NULL;
	char str[5];
	usart_get(str);
	shmid = shmget(key,sizeof(struct humi_ture),0666|IPC_CREAT);
	if(shmid<0)
	{
		perror("shmget error\n");
		exit(1);
	}			
	ht = (struct humi_ture*)shmat(shmid,(void*)0,0);
//	if(ht == NULL);
//	{
//		perror("shmat error\n");
//		exit(-1);
//	}
//	printf("%c %c %c %c \n",str[0],str[1],str[2],str[3]);
	ht->humi=(str[2]-'0')*10+(str[3]-'0');
	ht->ture=(str[0]-'0')*10+(str[1]-'0');
	//i++;j++;
	return 0;
}

struct humi_ture* get_shm(void)
{
	int shmid;
	key_t key = 0x2222;
	struct humi_ture* ht= NULL;
	
	shmid = shmget(key,sizeof(struct humi_ture),0666|IPC_CREAT);
	if(shmid<0)
	{
	//	perror("shmget error\n");
	//	exit(1);
	}			
	ht = (struct humi_ture*)shmat(shmid,(void*)0,0);
	if(ht == NULL);
	{
	//	perror("shmat error\n");
	//	exit(1);
	}
	return ht;
}


//-----------------------------------------------------------------
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
  struct termios newtio,oldtio;
  if ( tcgetattr( fd,&oldtio) != 0) { 
    perror("SetupSerial 1");
    return -1;
  }
  bzero( &newtio, sizeof( newtio ) );
  newtio.c_cflag |= CLOCAL | CREAD; 
  newtio.c_cflag &= ~CSIZE; 

  newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
  newtio.c_oflag  &= ~OPOST;   /*Output*/

  switch( nBits )
  {
  case 7:
    newtio.c_cflag |= CS7;
    break;
  case 8:
    newtio.c_cflag |= CS8;
    break;
  }

  switch( nEvent )
  {
  case 'O':
    newtio.c_cflag |= PARENB;
    newtio.c_cflag |= PARODD;
    newtio.c_iflag |= (INPCK | ISTRIP);
    break;
  case 'E': 
    newtio.c_iflag |= (INPCK | ISTRIP);
    newtio.c_cflag |= PARENB;
    newtio.c_cflag &= ~PARODD;
    break;
  case 'N': 
    newtio.c_cflag &= ~PARENB;
    break;
  }

switch( nSpeed )
  {
  case 2400:
    cfsetispeed(&newtio, B2400);
    cfsetospeed(&newtio, B2400);
    break;
  case 4800:
    cfsetispeed(&newtio, B4800);
    cfsetospeed(&newtio, B4800);
    break;
  case 9600:
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    break;
  case 115200:
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);
    break;
  default:
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    break;
  }
  if( nStop == 1 )
    newtio.c_cflag &= ~CSTOPB;
  else if ( nStop == 2 )
  newtio.c_cflag |= CSTOPB;
  newtio.c_cc[VTIME] = 0; /* 设置从获取到1个字节后开始计时的超时时间 */
  newtio.c_cc[VMIN] = 0;  /* 设置要求等待的最小字节数 */
  tcflush(fd,TCIFLUSH);
  if((tcsetattr(fd,TCSANOW,&newtio))!=0)
  {
    perror("com set error");
    return -1;
  }
  //printf("set done!\n");
  return 0;
}

int open_port(char *com)
{
	int fd;
	//fd = open(com, O_RDWR|O_NOCTTY|O_NDELAY);
	fd = open(com, O_RDWR|O_NOCTTY);
    if (-1 == fd){
		return(-1);
    }
	
  if(fcntl(fd, F_SETFL, 0)<0)
  {
		printf("fcntl failed!\n");
		return -1;
  }

  fcntl(fd, F_SETFL, 0);
  
  return fd;
}


void usart_get(char *c)
{
	int oldstate;
	int oldtype;
	int iRet;
	int fd = -1;
//	unsigned char c;
	int ok_cnt = 0;
	int err_cnt = 0;
	//char str[5];
	
	fd = open_port("/dev/s3c2410_serial1");
	if (fd < 0)
	{
		printf("open port err!\n");
		return ;
	}

	iRet = set_opt(fd, 9600, 8, 'N', 1);
	if (iRet)
	{
		printf("set port err!\n");
		return ;
	}

	printf("Data recv: \n");
	if (1)
	{
		char ch='T';
		//char str[5];
		int i;
		read(fd,&ch,1);
		while(ch != 'S')
		{
			read(fd,&ch,1);
		}
		ch='T';
		
		for(i=0;i<4;i++)
		{
			sleep(1);
			read(fd,&c[i],1);
			if(c[i]=='S')
			{
				i=-1;
			}
		}
		c[4]='\0';
		printf("%s\n",c);
		printf("%c %c %c %c\n",c[0],c[1],c[2],c[3]);
	}

	//return str;
}
