#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pub_sub_deliv.h"

void * deliver_1_svc(postmessage *pMessage, struct svc_req *pSvcReq){
    //printf("Entered deliver_1_svc\n");
    printf("%s\n",*pMessage);
}