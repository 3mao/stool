#include <stdio.h>
#include <string.h>

int main(int argc,char ** argv)
{
	FILE * fp;
	char line[100]={'\0'};
	char * buf;
	char * hinf[3]={NULL};
	char * user;
	char * password;
	char * ip;
	char * p;
	int i =0;
	int flag =0;
	fp = fopen("hlist.txt","r");
	
	if(NULL==fp)
	{
		fprintf(stderr,"fopen file failed\n");
		return -1;
	}


	for(;! feof(fp) && NULL != fgets(line,sizeof(line),fp);)
	{
		  buf=line;
   		 p = strsep(&buf, ", ");
    		for(i=0;p;i++)
   		 {

			if(p[strlen(p)-1]=='\n')	p[strlen(p)-1]='\0';
			hinf[i]=p;
       			 printf("%s---\n",hinf[i]);
        		p = strsep(&buf, ",");
    		}	
	};

	fclose(fp);
	return  0;
}
