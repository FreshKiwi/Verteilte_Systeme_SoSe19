#include <stdio.h>
#include "pub_sub.h"
#include "return_codes.h"


void setNewTopic(CLIENT* cl);
void subscribeToServer(CLIENT* cl);
void unsubscribeFromServer(CLIENT* cl);
void readInputToPublish(CLIENT* cl);

int main (int argc, char **argv){
    
    static CLIENT* cl;
    char userChoice;
    char* server;
    short* result;
    
    if(argc != 2){
        fprintf(stderr,"Serveradresse nicht lesbar/fehlt\n");
        exit(1);
    }
    server = argv[1];
    userChoice = 'x'; //Platzhalter
    cl = clnt_create(server, PUBSUBPROG, PUBSUBVERS,"tcp");
    if(cl == NULL){
        printf("Verbindungserstellung erfolglos\n");
        clnt_pcreateerror(server);
        exit(1);
    }
    
    
    printf("Starte den RPC-Client.\n");
    //Verbindung mit dem Server herstellen
    while(userChoice != '1'){
        //Nutzer nach Kommando fragen
        printf("Client-Kommandos:\n\t(1)\tBeenden\n\t(2)\tto set new topic\n");
        printf("\t(3)\tto subscribe to this server and listen with receiver\n");
        printf("\t(4)\tto unsubscribe from this server stop listening\n");
        printf("\t(5)\tto enter the publish-interface.\n");
//        getchar();  //Überbliebenen newline Char entfernen;
        
        userChoice = fgetc(stdin);
        fflush(stdin);
        printf("User Input: %c\n",userChoice);

        switch(userChoice){
            case '1': printf("Quitting Client");exit(0);break;
            case '2': setNewTopic(cl);break;
            case '3': subscribeToServer(cl);break;
            case '4': unsubscribeFromServer(cl);break;
            case '5': readInputToPublish(cl);break;
            default: printf("Couldn't read input properly\n");break;
        }
    }
    return 0;
}

void setNewTopic(CLIENT* cl){
    static char newTopic[TOPLEN];
    static topic topicChar;
    topicChar = "Platzhalter";
    short* returnMessage;
    
    getchar();  //Überbliebenen newline Char entfernen;
    printf("Set new Topic:\n");
    
    if(fgets(newTopic,TOPLEN,stdin) != NULL){
        topicChar = newTopic;
        returnMessage = (short*) set_channel_1(&topicChar,cl);
        printf("Serverantwort: %s\n",PUB_SUB_RET_CODE[*returnMessage]);
    } else {
        printf("Couldn't read input properly\n");
    }
}
void subscribeToServer(CLIENT* cl){ // get zum Server hat dann aber einen Speicherzugriffsfehler
    printf("Subscribe to the Server\n");
    short *returnMessage;
        
    returnMessage = subscribe_1(0,cl);
    getchar();  //Überbliebenen newline Char entfernen;
}
void unsubscribeFromServer(CLIENT* cl){ // get zum Server hat dann aber einen Speicherzugriffsfehler
    printf("Unsubscribe from the Server\n");
    //Only unsubscribe when subscribe anywhere!
    short *returnMessage;
        
    returnMessage = unsubscribe_1(0,cl);
    getchar();  //Überbliebenen newline Char entfernen;
}
void readInputToPublish(CLIENT* cl){
    static char newPublish[TOPLEN];
    static message messageChar;
    messageChar = "Platzhalter";
    short* returnMessage;
    getchar();  //Überbliebenen newline Char entfernen;
    printf("Input your publish:\n");
    if(fgets(newPublish,TOPLEN,stdin) != NULL){
        messageChar = newPublish;
        printf("User Input: %s\n",newPublish);
        returnMessage = publish_1(&messageChar,cl);
        printf("Serverantwort: %s\n",PUB_SUB_RET_CODE[*returnMessage]);
    } else {
        printf("Couldn't read input properly\n");
    }
}