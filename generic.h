#ifndef _GENERIC_H_
#define _GENERIC_H_

//#define PTW32_STATIC_LIB

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<getopt.h>    //strlen
//#include<pthread.h> //for threading , link with lpthread

#include<unistd.h>


#ifdef WIN32
	#include<winsock2.h>
	#include<ws2tcpip.h>
    #include<windows.h>
#else
	#include <netdb.h>
	#include<sys/socket.h>
	#include<arpa/inet.h>
	#include<signal.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>
    #include<termios.h>
    #include<errno.h>
#endif // head files check

// my headers start from here
#include "Serial_Base_API.h"
// end my headers


#ifdef _DEBUG_PRINT_
    #define Printf(fmt, args...) printf( fmt,##args )
#else 
    #define Printf(fmt, args...) ;
#endif

#endif // _GENERIC_H_
