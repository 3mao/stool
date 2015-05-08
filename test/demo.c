#include <stdio.h>
#include<string.h>
int main()
{
	char buffer[10];
	int  n =0;

	n = read(0,buffer,1024);
	
	fprintf(stderr,"n=%d,buffer=%s",n,buffer);

	

}
