#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "io.h"

int 
main(int argc, char ** argv)
{

	int	in = STDIN_FILENO;
	int	out= STDOUT_FILENO;

	int 	ret = 0;
	int  	rc = 0;
	char buffer[BUFFSIZE]={0};

	printf("设置非阻塞之前\n");
	rc = read(in,buffer,BUFFSIZE);
	
	switch(rc)
	{
		case 0:
			printf("读取结束\n");
			break;
		case -1:
			printf("读取到结束\n");
			break;
		default:
			printf("rc=%d,content:%s",rc,buffer);
			break;
	}

	if(0 < SET_FL(in,O_NONBLOCK))
	{

		printf("设置非阻塞方式失败,函数返回\n");
		return 1;
	}

	printf("设置非阻塞成功\n");	
	memset(buffer,0,BUFFSIZE);
	
	printf("wait fd\n");	
	ret = waitfd(in,3,0);

        if(-1 == ret )
        {
                printf("select 出错,errno:%d,errmsg:%s",errno,strerror(errno));
        }else if(0 == ret)
        {
                printf("select time out\n");
        }else{

         rc = read(in,buffer,BUFFSIZE);
         if(rc < 0 ) printf("read return errno:%d,erromsg:%s\n",errno,strerror(errno));

         printf("rc=%d,content:%s\n",rc,buffer);
        }

	if(0 < CLR_FL(in,O_NONBLOCK))
	{
		printf("清除非阻塞失败\n");
		return  0;
	};

	printf("设置阻塞成功\n");
	 memset(buffer,0,BUFFSIZE);
	rc = read(in,buffer,BUFFSIZE);
	if(rc < 0 ) printf("read return errno:%d,erromsg:%s\n",errno,strerror(errno));
	printf("rc=%d,content:%s\n",rc,buffer);

	return  0;
}


int  waitfd(int maxfd,int sec,int usec)
{
	struct timeval tim;
	fd_set set;
	int ret = 0;
	
	FD_ZERO(&set);
	FD_SET(maxfd,&set);
	tim.tv_sec=sec;
	tim.tv_usec=usec;
	return select(maxfd+1,&set,NULL,NULL,&tim);

}
