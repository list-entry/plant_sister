#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "config.h"
#include "ipc.h"
#include "malloc.h"
//char str[5];//=(char*)malloc(sizeof(char)*5);
int main(void)
{
	while(1)
	{
		rcv_req();
	}	
}
