#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define EPOLLSIZE	10
#define MAX_EVENTS 	10
struct epoll_event g_epevents[MAX_EVENTS];

int main(int argc,char ** argv)
{

	int epfd=-1;
	int ret = -1;
	int op = -1;
	int i = 0;
	struct epoll_event epv={0,{0}};
	struct epoll_event * ep;


	epfd=epoll_create(EPOLLSIZE);

	if(-1 == epfd)	
	{
		fprintf(stderr,"epoll_create tailed,errno=%d,errmsg:%s",errno,strerror(errno));
	}
	
	op = EPOLL_CTL_ADD;
	epv.events = EPOLLIN;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&epv);

	ret = epoll_wait(epfd,g_epevents,MAX_EVENTS,1000);


	if(0 == ret)
	{
		fprintf(stderr,"epoll_wait timeout\n");
	}
	else if(-1 == ret)
        {
                fprintf(stderr,"epoll_wait tailed,errno=%d,errmsg:%s",errno,strerror(errno));
        }	
		
	for(i=0; i<ret; i++)
	{
		if(g_epevents[i].events & EPOLLIN)
		{
			fprintf(stdout,"has read events\n");
		}
	}
}
