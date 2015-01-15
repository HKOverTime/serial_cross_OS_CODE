#ifndef _SERIAL_BASE_API_H_
#define _SERIAL_BASE_API_H_

#include "generic.h"

#ifdef WIN32
    #define SERIAL_HANDLE_ID HANDLE
    #define SERIAL_HANDLE_ID_ERROR (HANDLE)(-1)
#else 
    #define SERIAL_HANDLE_ID int
    #define SERIAL_HANDLE_ID_ERROR (-1)
    #define FALSE               -1
    #define TRUE                 0
    typedef unsigned long DWORD;
#endif // 
#define SERIAL_MAX_PACKAGE   256 // 65535
#define SERIAL_INIT_ENV_OK   1
#define SERIAL_SEND_ERROR   -1
#define SERIAL_RECV_ERROR   -1


int API_init_serial_env();
SERIAL_HANDLE_ID API_Serial_Open(char *Dev,int speed,
        int databits,int stopbits);
    //for linux   ("/dev/ttyS1",19200,8,1)   
    //for windows ("COM1",      19200,8,1)
    // 错误时返回 SERIAL_HANDLE_ID_ERROR;
DWORD API_Serial_Send(SERIAL_HANDLE_ID id, char *buffer,DWORD buflen);
    // 正确时返回发送的字节数，错误时返回 SERIAL_SEND_ERROR
DWORD API_Serial_recv(SERIAL_HANDLE_ID id, char *buffer,DWORD buflen);
    // 正确时返回收到的字节数，错误时返回 SERIAL_RECV_ERROR
int API_Serial_close(SERIAL_HANDLE_ID id);


#endif
