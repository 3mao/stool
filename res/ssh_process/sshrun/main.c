#ifndef _MAIN_C_
#define  _MAIN_C_

#include"main.h"


int main(int argc, char *argv[])
{
        char buffer[BUFFER_SIZE]={0};
        SSH  ssh_t;

        int fd_in=STDIN_FILENO;
        int fd_out=STDOUT_FILENO;
        int n_read=0;
	int  i =0;


        ssh_init(&ssh_t,argv[1],argv[2],argv[3]);
	if(5==argc) 
	{
		if(0>(fd_in=open(argv[4],O_RDONLY)))
        	{
                	fprintf(stderr,"open:%s",strerror(errno));
                	fd_in=STDIN_FILENO;
        	}
        }
             

	while(1)
	{
		n_read=read(fd_in,buffer,BUFFER_SIZE);
        	if(n_read<=0)
        	{
                	fprintf(stderr,"fd=%d,read:%s",fd_in,strerror(errno));
			break;
        	}

		
                ssh_cmd(&ssh_t,buffer);

	}
        ssh_free(&ssh_t);
	close(fd_in);
        exit( 0);
}

#endif //end define MAIN.C
