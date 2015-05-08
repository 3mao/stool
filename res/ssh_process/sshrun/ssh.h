#ifndef _SSH_H_
#define _SSH_H_

#ifdef	_SSH_C_
#define	EXTERN 
#else
#define EXTERN extern
#endif

#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif /*end extern "C" { */ 

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#include<libssh2.h>
	typedef struct SSH
	{
		int sock_fd;
		struct sockaddr_in sin;
		LIBSSH2_SESSION *session;
		LIBSSH2_CHANNEL *channel;
		LIBSSH2_KNOWNHOSTS *nh;
		
	}SSH,*PSSH;

EXTERN int ssh_init(SSH *,char * , char * , char *);
EXTERN int ssh_cmd(SSH *,char *);
EXTERN int ssh_free(SSH *);
static int waitsocket(int socket_fd, LIBSSH2_SESSION *session);

#if defined(__cplusplus)||defined(c_plusplus)
}
#endif /*end extern "C" of } */ 

#endif  /*end _SSH_H_*/
