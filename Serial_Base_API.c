#include "Serial_Base_API.h"

#ifdef WIN32

HANDLE win32_OpenComm(char *Dev,int speed,
        int databits,int stopbits){
    HANDLE hCom;  
    hCom = CreateFile(Dev,//COM1?
                       GENERIC_READ|GENERIC_WRITE, 
               0, 
               NULL,
               OPEN_EXISTING, 
               0,//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
               NULL);
    if(hCom==(HANDLE)(-1))
    {
        printf("Error :  COM Create !\n");
        return (HANDLE)(-1);
    }
    SetupComm(hCom,102400,102400); 
         
    COMMTIMEOUTS TimeOuts;  
          
    TimeOuts.ReadIntervalTimeout=1000;  
    TimeOuts.ReadTotalTimeoutMultiplier=500;  
    TimeOuts.ReadTotalTimeoutConstant=5000;  
    /* 
        TimeOuts.WriteTotalTimeoutMultiplier=500;  
        TimeOuts.WriteTotalTimeoutConstant=2000;  
 */    SetCommTimeouts(hCom,&TimeOuts);  
        
        DCB dcb;  
        GetCommState(hCom,&dcb);  
        dcb.BaudRate=speed;  
        dcb.ByteSize=databits; 
        dcb.Parity=NOPARITY; 
        dcb.StopBits=stopbits ; 
        SetCommState(hCom,&dcb);  
         
        PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);  
        return hCom;
}


#else
int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
        B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400,  19200,  9600,  4800,  2400,  1200,  300,
        38400,  19200,  9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed)
{
  int   i;
  int   status;
  struct termios   Opt;
  tcgetattr(fd, &Opt);
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
   {
    if  (speed == name_arr[i])
    {
        tcflush(fd, TCIOFLUSH);
        cfsetispeed(&Opt, speed_arr[i]);
        cfsetospeed(&Opt, speed_arr[i]);
        status = tcsetattr(fd, TCSANOW, &Opt);
        if  (status != 0)
            perror("tcsetattr fd1");
        return;
        }
   tcflush(fd,TCIOFLUSH);
   }
}

/**
*/
int set_Parity(int fd,int databits,int stopbits,int parity)
{
    struct termios options;
 if  ( tcgetattr( fd,&options)  !=  0)
  {
    perror("SetupSerial 1");
    return(FALSE);
  }
  options.c_cflag &= ~CSIZE;
  switch (databits) /*ÉèÖÃÊýŸÝÎ»Êý*/
  {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        return (FALSE);
    }
  switch (parity)
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;   /* Clear parity enable */
        options.c_iflag &= ~INPCK;     /* Enable parity checking */
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);   
        options.c_iflag |= INPCK;             
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;     /* Enable parity */
        options.c_cflag &= ~PARODD;     
        options.c_iflag |= INPCK;       /* Disnable parity checking */
        break;
    case 'S':
    case 's':  /*as no parity*/
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
        return (FALSE);
        }

  switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        return (FALSE);
    }
  /* Set input parity option */
  if (parity != 'n')
    options.c_iflag |= INPCK;
    options.c_cc[VTIME] = 150; // 15 seconds
    options.c_cc[VMIN] = 0;

  tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
  if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
  return (TRUE);
 }

/**
*/
int OpenDev(char *Dev)
{
int fd = open( Dev, O_RDWR );         //| O_NOCTTY | O_NDELAY
    if (-1 == fd)
        { /*ÉèÖÃÊýŸÝÎ»Êý*/
            perror("Can't Open Serial Port");
            return -1;
        }
    else
    return fd;

}
#endif


int API_init_serial_env(){
    return SERIAL_INIT_ENV_OK;
}

SERIAL_HANDLE_ID API_Serial_Open(char *Dev,int speed,
        int databits,int stopbits){
    SERIAL_HANDLE_ID id;
#ifdef WIN32
    
    id =  win32_OpenComm(Dev, speed,
         databits, stopbits);
    return id;
#else
    id = OpenDev(Dev);
    if (id>0)
        set_speed(id,speed);
    else {
        printf("Can't Open Serial Port!\n");
        return SERIAL_HANDLE_ID_ERROR;
    }
    if (set_Parity(id,databits,stopbits,'N') == FALSE) {
        printf("Set Parity Error\n");
        return SERIAL_HANDLE_ID_ERROR;
    }
#endif
    return id;
}

DWORD API_Serial_Send(SERIAL_HANDLE_ID id, char *buffer,DWORD buflen){
    DWORD nwrite = buflen;
#ifdef WIN32
    BOOL bWriteStat;
    bWriteStat = WriteFile(id , buffer,buflen,&nwrite,NULL);
    if(!bWriteStat){
        printf("Error in WriteFile function !\n");
        return SERIAL_SEND_ERROR;
    }
#else
    nwrite = write(id,buffer,strlen(buffer));
    if(nwrite != buflen){
        return SERIAL_SEND_ERROR;
    }
#endif
    return nwrite;
}

DWORD API_Serial_recv(SERIAL_HANDLE_ID id, char *buffer,DWORD buflen){
    DWORD nrecv = buflen; 
#ifdef WIN32
    BOOL bReadStat;
    bReadStat = ReadFile(id,buffer,buflen,&nrecv,NULL);  
    if(!bReadStat)  
    {  
        printf("Error : read file !\n");  
        return SERIAL_RECV_ERROR;
    }  
#else
    nrecv = read(id,buffer,buflen);
    if (nrecv <0){
        return SERIAL_RECV_ERROR;
    }
#endif
    return nrecv;
}

    // 正确时返回收到的字节数，错误时返回 SERIAL_RECV_ERROR
int API_Serial_close(SERIAL_HANDLE_ID id){
#ifdef WIN32
    CloseHandle(id);
#else
    close(id);
#endif
    return 1;
}
