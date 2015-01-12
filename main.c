#include "generic.h"

#ifdef WIN32
    char DevName[100] = "COM2";
#else
    char DevName[100] = "/dev/ttyS1";
#endif

int main(){
    SERIAL_HANDLE_ID id ;
    id = API_Serial_Open(DevName,19200,
            8,1);
    if (id == SERIAL_HANDLE_ID_ERROR){
        printf("Serial Handle open error\n");
        exit(0);
    }
    char buffer[200] = "Hello Serial\n";
    char recvbuf[1024] ;
    int nrecv,nwrite;
    while ((nrecv = API_Serial_recv(id,recvbuf,
            1023)) != SERIAL_RECV_ERROR){
        recvbuf[nrecv] = '\0';
        printf("\nrecv [%d] : %s",nrecv,recvbuf);
        nwrite = API_Serial_Send(id,buffer,strlen(buffer));
        printf("\nwrite [%d] : %s \n",nwrite,buffer);
    }
}
