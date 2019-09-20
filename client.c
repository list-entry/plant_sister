#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "config.h"
#include "ipc.h"

void refresh(struct humi_ture*ht);

int main(void)
{
	struct humi_ture* ht=(struct humi_ture*)malloc(sizeof(struct humi_ture));
	int req;
	ht->humi=10;
	ht->ture=10;
	req = snd_req();
	if(req != 0)
	{
	//	perror("snd_req error");
	//	exit(1);
	}	
	sleep(2);
	ht = get_shm();
	refresh(ht);
}
void refresh(struct humi_ture*ht)
{
	printf("Content-type:text/html;charset=utf-8\n\n");

	printf("<meta http-equiv='Refresh' content='7'>\n");
	printf("<meta http-equiv='Pragma' content='no-cache'>\n");
	printf("<meta http-equiv='no-cache'>\n");
	printf("<meta http-equiv='Expires' content='-1'>\n");
	printf("<meta http-equiv='Cache-Control' content='no-cache'>\n");
	
        printf("<html>\n");
        printf("<head><title>植宠保姆</title></head>\n");
        printf("<body>\n");
        printf("<h1>温度:%0.2f</h1>\n",ht->ture);
        printf("<h1>湿度:%0.2f</h1>\n",ht->humi);
        printf("<body>\n");
        printf("</html>\n");
}
