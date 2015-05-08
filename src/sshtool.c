#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

	extern   char **  environ;
//	struct utsname  g_u;  

	typedef struct  Proc
	{
		int ActiveProc;
		int ZombieNum;
		int HasProc;

	}Proc,*pProc;

	Proc   proc={0,0,0};
	int ActiveProcMax=10;
	
	struct utsname 	uts;

	void	sig_chld(int sig_num);
	void	DisplayProc(void);
	void	ZombieHandle(void);

int main(int argc,char ** argv)
{
	pid_t  pid;
	int 	ret=0;
	int i =0;
	char buffer[BUFSIZ]={0};
	char ip[20]={0};
	char user[20]={0};
	char pwd[20]={0};
	char constr[100]={0};
	char path[256]={0};
	char cwd[256]={0};
	char utype[20]={0};

	int  fd_out = open("../res/cmd.sh",O_WRONLY,755);
	FILE *  fp = fopen(argv[1],"r");

	

	signal(SIGCHLD,sig_chld);

	read(STDIN_FILENO,buffer,BUFSIZ);
	write(fd_out,buffer,BUFSIZ);
	close(fd_out);
	
	getcwd(cwd,sizeof(cwd));
	uname(&uts);
	
	sprintf(path,"%s/%s/%s",cwd,uts.sysname,"stool");
	
	printf("path:%s\n",path);
	for(i=0; EOF != fscanf(fp,"%s\t%s\t%s",ip,user,pwd); i++)
	{

		fprintf(stderr,"%s %s %s\n",ip,user,pwd);
		pid = fork();

		if(pid<0)
		{
			printf("fork failed\n");
		}	
		
		if(pid==0)
		{

			//sprintf(pwd,"-p %s",pwd);
			sprintf(constr,"%s@%s",user,ip);
			ret=execlp(path,"stool","-p",pwd,"ssh",constr,"-o","ConnectTimeout=1","-o","StrictHostKeyChecking=no",buffer,NULL);
			if(ret==-1)
			{
				printf("execl failed:%s\n",strerror(errno));
			}	
		}else if(pid>0)
		{
			
			
		
		}		

		proc.HasProc ++;
		proc.ActiveProc ++;
		

		while(ActiveProcMax <= proc.ActiveProc)
		{

			ZombieHandle();
		}

			DisplayProc();		

	}

	         while(proc.ZombieNum>0 || proc.ActiveProc>0)
                {
		//	fprintf(stderr,"proc.ActiveProc=%d\n",proc.ActiveProc);
                        ZombieHandle();
                }

	return  0;

}


void   sig_chld(int sig_num)
{
	int status;

	proc.ZombieNum ++;
	proc.ActiveProc --;
	
}


void DisplayProc(void)
{

	fprintf(stderr,"proc.HasProc:\t%d\n",proc.HasProc);
	fprintf(stderr,"proc.ActiveProc:\t%d\n",proc.ActiveProc);
	fprintf(stderr,"proc.ZombieNum:\t%d\n",proc.ZombieNum);
}


void  ZombieHandle(void)
{
	
		
	while( waitpid(-1,0,WNOHANG) > 0 &&  proc.ZombieNum)
	{
		 proc.ZombieNum --;
	}
			
		

	

}
