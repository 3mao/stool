#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv)
{
	int i = 0;

	for(i=0; i<2; i++)
	{

	  sleep(1);
	  printf("ppid=%d,pid=%d,the seconds: %d \n",getppid(),getpid(),i);

	}

	for(i=0; i<argc; i++)
	{
		printf("argument:%d is %s\n",i,argv[i]);
	}

	exit(0);
}
