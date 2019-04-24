#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "return_codes.h"
#include "pub_sub.h"
#include "pub_sub_deliv.h"


//Hier ne Liste
struct adress{
    char* clientIp;
    struct adress* next;
};
typedef struct adress adress;


adress* searchForIp(char* ipToSearch);
void printAdresses();
short delete(adress* toDelete);
void deliverToAll(postmessage *pMessage);

static adress* global_list = NULL;
static short globalResponse = OK;
static char currentTopic[TOPLEN] = "PLATZHALTER";
//static topic * currentTopic;

short * set_channel_1_svc(topic *pTopic, struct svc_req *pSvcReq){  //Scheint so zu funktionieren
    //Setze Ausgabechannel
    printf("SetChannel called!\n");
    char* req_addr = inet_ntoa(pSvcReq->rq_xprt->xp_raddr.sin_addr);
    if(strcmp(req_addr,"127.0.0.1") == 0){
        printf("Client from the same Host as the server\n");
        sprintf(currentTopic,"%s",*pTopic); //Fehler ignorieren
        printf("Channel Topic now is: %s\n",currentTopic);
        strtok(currentTopic, "\n");
        globalResponse = OK;
    } else {//Client nicht auf dem gleichen Host = darf keinen Channel setzen
        globalResponse = CANNOT_SET_TOPIC;
    }
    return &globalResponse;
}
short * unsubscribe_1_svc(void *pPointer, struct svc_req *pSvcReq){
    printf("unsubscribe wurde aufgerufen!\n");
    char* req_addr = inet_ntoa(pSvcReq->rq_xprt->xp_raddr.sin_addr);
    adress* toDelete = (adress*)malloc(sizeof(adress));
    toDelete = searchForIp(req_addr);
    globalResponse = delete(toDelete);
    printf("Nach delete: %s\n",PUB_SUB_RET_CODE[globalResponse]);
    printf("Listeninhalt nach delete:\n");
    printAdresses();
    return &globalResponse;
}

short delete(adress* toDelete){
    adress* tmp = NULL;
    if((global_list == NULL) || toDelete == NULL){
        return CANNOT_UNREGISTER;
    }
    if(strcmp(toDelete->clientIp, global_list->clientIp)==0){
        global_list = toDelete->next;
        free(toDelete);
    } else {
        tmp = global_list;
        while(strcmp(tmp->next->clientIp,toDelete->clientIp)!= 0){
            tmp = tmp->next;
        }
        tmp->next = toDelete->next;
        free(toDelete);
    }
    return OK;
}

short * subscribe_1_svc(void *pPointer, struct svc_req *pSvcReq){
    printf("subscribe wurde aufgerufen!\n");
    char* req_addr = inet_ntoa(pSvcReq->rq_xprt->xp_raddr.sin_addr);
    globalResponse = OK;
    //adress* listenAnfang = global_list;
    adress* temp = (adress*) malloc(sizeof(adress));   //Kein Ausweg vor malloc!
    adress* findAdr;
    
    if(global_list == NULL){
        printf("Registering client.\n");
        temp->clientIp = strdup(req_addr);
        //temp->clientIp = req_addr;
        temp->next = NULL;
        global_list = temp;
        globalResponse = OK;
    }else{
        printf("Testing if client is already registered or not..\n");
//        printf("List before Search: \n");
//        printAdresses();
        findAdr = searchForIp(req_addr);
        if(findAdr != NULL){            //Client already registered
            printf("Client Already Registered\n");
            globalResponse = CLIENT_ALREADY_REGISTERED;
        } else {
            printf("Not registered so far.\n");
            findAdr = global_list;
            /*Noch falsch*/
            while(findAdr->next != NULL){
                findAdr = findAdr->next;
            }
            temp->clientIp = strdup(req_addr);
            temp->next = NULL;
            findAdr->next = temp;
            printf("Current ElementsA: \n");
            printAdresses();
//            global_list = temp;
            globalResponse = OK;
        }
    }
    printf("Current Elements: \n");
    printAdresses();
    return &globalResponse;
}
short * publish_1_svc(message *pMessage, struct svc_req *pSvcReq){
    strtok(*pMessage, "\n");
    //deliverToAll(pMessage);
    globalResponse = OK;
    static CLIENT* cl;
    char *ptr;
    message send;
    char * fullMessage[POSTMESLEN];
//    printf("Aktuelle Elemente: \n");
//    printAdresses();
    char* req_addr = inet_ntoa(pSvcReq->rq_xprt->xp_raddr.sin_addr);
    
    sprintf(fullMessage, "%s: %s", currentTopic, *pMessage);
    
    adress* tmp;
    tmp = global_list;
    if(searchForIp(req_addr)!=NULL){
        while(tmp != NULL){
            printf("Versuche Verbindung mit Client auf %s\n",tmp->clientIp);
            cl = clnt_create(tmp->clientIp, PUBSUBCLTPROG, PUBSUBVERS,"tcp");

            if(cl == NULL){
                printf("Verbindungserstellung erfolglos\n");
                clnt_pcreateerror(tmp->clientIp);
                globalResponse = UNKNOWN_ERROR;
            }else {
                printf("Delivering to Client %s: %s\n",tmp->clientIp, fullMessage);

                struct timeval tv;
                tv.tv_sec = tv.tv_usec = 0;
                clnt_control(cl, CLSET_TIMEOUT, &tv);

                send = fullMessage;
                deliver_1(&send,cl);
            }
            tmp = tmp->next;
        }
    } else {
        printf("Client isn't subscribed!\n");
        globalResponse = UNKNOWN_ERROR;
    }
    
    
    return &globalResponse;
}

adress* searchForIp(char* ipToSearch){
    if(global_list != NULL){
        adress * tmp = global_list;
        while(tmp!=NULL){
            if(strcmp(tmp->clientIp,ipToSearch)==0){
                printf("%s is the same IP as %s\n",tmp->clientIp,ipToSearch);
                return tmp;
            } else {
                tmp = tmp->next;
            }
        }
    } else {
        return NULL; //Adress not found
    }
}

void printAdresses(){
    adress * tmp = global_list;
    int runner = 0;
    while(tmp!=NULL){
        printf("Element: %d; Adresse: %s\n",runner,tmp->clientIp);
        tmp = tmp->next;
        runner++;
    }
    printf("Print Ende \n\n");
}