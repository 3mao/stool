#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

int  putmemhex(unsigned char * p,int n)
{
        int i=0;
		int j=0;
        if(p==NULL)
        {
                return -1;
        }
        for(i=0,j=1;i<n;p++,i++,j++)
        {

                printf("%.2X ",*(unsigned char *)p);
				if(0==j%40)
				{
					printf("\n");
					continue;
				}

				if(0==j%4)
				{
					printf("  ");
				}

        }
        printf("\n");

        return 0;
}

int main(int argc,char ** argv)
{
	
	fd_set rset;
	struct timeval t;
	int ret=0;

	t.tv_sec=10;
	t.tv_usec=0;
		
	FD_ZERO(&rset);

	putmemhex((unsigned char *)&rset,sizeof(rset));

	FD_SET(STDIN_FILENO,&rset);


	switch((ret=select(STDIN_FILENO+1,&rset,NULL,NULL,&t)))
	{
		case -1:
			printf("-1 is exit\n");
			break;
		case 0:
			printf("select timeout\n");
			break;
		default:
			printf("is ok\n");
			break;
	}		
	printf("\n\n");
	 putmemhex((unsigned char *)&rset,sizeof(rset));
}
