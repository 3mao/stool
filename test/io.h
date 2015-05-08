#ifndef  _IO_H_
#define  _IO_H_

#define SET_FL(fd,flag)  \
        fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | flag)
#define CLR_FL(fd,flag) \
        fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) & (~flag))
#define BUFFSIZE        1024

#endif  //end define #define  _IO_H_
