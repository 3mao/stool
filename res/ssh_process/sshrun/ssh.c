#ifndef _SSH_C_
#define _SSH_C_


#include <ssh.h>


int ssh_init(SSH * pSsh, char * pIp,char * pUsername, char * pPassword)
{
	int ret;
	int type;
	const char * fingerprint;
	 size_t len;
	if (NULL==pSsh)
	{
		return 0;
	}
	
	pSsh->sock_fd=socket(AF_INET, SOCK_STREAM, 0);
	pSsh->sin.sin_family = AF_INET;
	pSsh->sin.sin_port = htons(22);
	pSsh->sin.sin_addr.s_addr = inet_addr(pIp);

	ret = libssh2_init (0);
	if (ret != 0) {
		fprintf (stderr, "libssh2 initialization failed (%d)\n", ret);
		return 1;
	}

	if (connect(pSsh->sock_fd, (struct sockaddr*)(&pSsh->sin),
		sizeof(struct sockaddr_in)) != 0) {
			fprintf(stderr, "failed to connect!\n");
			return -1;
	}

	pSsh->session=libssh2_session_init();

	if (!pSsh->session)
		return 0;

	libssh2_session_set_blocking(pSsh->session, 0);

	while ((ret = libssh2_session_handshake(pSsh->session, pSsh->sock_fd)) ==
		LIBSSH2_ERROR_EAGAIN);
	if (ret) {
		fprintf(stderr, "Failure establishing SSH session: %d\n", ret);
		return -1;
	}


	pSsh->nh = libssh2_knownhost_init(pSsh->session);
	if(!pSsh->nh) {
		/* eeek, do cleanup here */
		return 2;
	}

	libssh2_knownhost_readfile(pSsh->nh, "known_hosts",
		LIBSSH2_KNOWNHOST_FILE_OPENSSH);
	libssh2_knownhost_writefile(pSsh->nh, "dumpfile",
		LIBSSH2_KNOWNHOST_FILE_OPENSSH);

	fingerprint = libssh2_session_hostkey(pSsh->session, &len, &type);
	if(fingerprint) {
		struct libssh2_knownhost *host;
		int check = libssh2_knownhost_checkp(pSsh->nh, pIp, 22,
			fingerprint, len,
			LIBSSH2_KNOWNHOST_TYPE_PLAIN|
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			&host);
		fprintf(stderr, "Host check: %d, key: %s\n", check,
                (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
                host->key:"<none>");

        /*****
         * At this point, we could verify that 'check' tells us the key is
         * fine or bail out.
         *****/
    }
    else {
        /* eeek, do cleanup here */
        return 3;
    }
    libssh2_knownhost_free(pSsh->nh);

    if ( strlen(pPassword) != 0 ) {
        /* We could authenticate via password */
        while ((ret = libssh2_userauth_password(pSsh->session, pUsername, pPassword)) ==
               LIBSSH2_ERROR_EAGAIN);
        if (ret) {
            fprintf(stderr, "Authentication by password failed.\n");
            //goto shutdown;
        }
    }
    else {
        /* Or by public key */
        while ((ret = libssh2_userauth_publickey_fromfile(pSsh->session, pUsername,
                                                         "/home/user/"
                                                         ".ssh/id_rsa.pub",
                                                         "/home/user/"
                                                         ".ssh/id_rsa",
                                                         pPassword)) ==
               LIBSSH2_ERROR_EAGAIN);
        if (ret) {
            fprintf(stderr, "\tAuthentication by public key failed\n");
            //goto shutdown;
        }
    }

	fprintf(stderr,"ssh_init\n");
	return 0;
}

int ssh_cmd(SSH * pSsh,char * pCmdstr)
{
		int ret=0;
		int exitcode;
		char *exitsignal=(char *)"none";

	if (NULL==pSsh || NULL==pCmdstr)
	{
		return 0;
	}

	while( (pSsh->channel = libssh2_channel_open_session(pSsh->session)) == NULL &&
		libssh2_session_last_error(pSsh->session,NULL,NULL,0) ==
		LIBSSH2_ERROR_EAGAIN )
	{
		waitsocket(pSsh->sock_fd, pSsh->session);
	}
	if( pSsh->channel == NULL )
	{
		fprintf(stderr,"Error\n");
		exit( 1 );
	}
	while( (ret = libssh2_channel_exec(pSsh->channel, pCmdstr)) ==
		LIBSSH2_ERROR_EAGAIN )
	{
		waitsocket(pSsh->sock_fd, pSsh->session);
	}
	if( ret != 0 )
	{
		fprintf(stderr,"Error\n");
		exit( 1 );
	}

	for( ;; )
    {
        /* loop until we block */
        int rc;
        do
        {
            char buffer[0x4000];
            rc = libssh2_channel_read( pSsh->channel, buffer, sizeof(buffer) );
            if( rc > 0 )
            {
                int i;
                
                for( i=0; i < rc; ++i )
                    fputc( buffer[i], stdout);
                fprintf(stderr, "\n");
            }
            else {
                if( ret != LIBSSH2_ERROR_EAGAIN )
                    /* no need to output this for the EAGAIN case */
                    fprintf(stderr, "libssh2_channel_read returned %d\n", rc);
            }
        }
        while( ret > 0 );

        /* this is due to blocking that would occur otherwise so we loop on
           this condition */
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(pSsh->sock_fd, pSsh->session);
        }
        else
            break;
    }
    exitcode = 127;
    while( (ret = libssh2_channel_close(pSsh->channel)) == LIBSSH2_ERROR_EAGAIN )
        waitsocket(pSsh->sock_fd, pSsh->session);

    if( ret == 0 )
    {
        exitcode = libssh2_channel_get_exit_status( pSsh->channel );
        libssh2_channel_get_exit_signal(pSsh->channel, &exitsignal,
                                        NULL, NULL, NULL, NULL, NULL);
    }

    if (exitsignal)
        fprintf(stderr, "\nGot signal: %s\n", exitsignal);
   

	fprintf(stderr,"\n*ssh_cmd::pcmdstr=%s\n",pCmdstr);


	return 0;
}


int ssh_free(SSH * pSsh)
{
	if (NULL==pSsh)
	{
		return 0;
	}
	pSsh->channel=NULL;
	libssh2_session_disconnect(pSsh->session,"Normal Shutdown, Thank you for playing");
	libssh2_session_free(pSsh->session);

	close(pSsh->sock_fd);
	fprintf(stderr, "all done\n");

	libssh2_exit();

	return 1;
}


static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */
	dir = libssh2_session_block_directions(session);

	if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}



#endif 
