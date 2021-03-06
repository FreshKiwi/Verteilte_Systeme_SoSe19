/* 
 Beispiel einfacher UDP Echo-Server 
 Aus Stevens: Unix Network Programming 
 CWe 10/2016: adaptiert fuer Ubuntu 16.04 64Bit
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SRV_PORT 8998
#define MAX_SOCK 10
#define MAXLINE 512
#define MAXSESSIONS 5

// Vorwaertsdeklarationen
void dg_echo(int); 
void err_abort(char *str);
struct session{
	int sockfd;
	int session_key;
	int chunksize;
	int chunk;
	char * filename;
	FILE * file;
};
typedef struct session Session;
// Explizite Deklaration zur Vermeidung von Warnungen
void exit(int code);
void *memset(void *s, int c, size_t n);


int main(int argc, char *argv[]) {

	// Deskriptor
	int sockfd;

	// Socket Adresse
	struct sockaddr_in srv_addr;

	// TCP-Socket erzeugen
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		err_abort("Kann Stream-Socket nicht oeffnen!");
	}

	// Binden der lokalen Adresse damit Clients uns erreichen
	memset((void *)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(SRV_PORT);
	if( bind(sockfd, (struct sockaddr *)&srv_addr,
		sizeof(srv_addr)) < 0 ) {
		err_abort("Kann lokale Adresse nicht binden, laeuft fremder Server?");
	}

	printf("UPD Echo-Server: bereit ...\n");
	
	dg_echo(sockfd);
} 

/* 
dg_echo: Lesen von Daten vom Socket und an den Client zuruecksenden 
*/
void dg_echo(int sockfd) {
	int alen, n, read;
	char in[MAXLINE], out[MAXLINE+6], tmp[MAXLINE], msg[MAXLINE];
	struct sockaddr_in cli_addr;
	char * tok;
	static int key= 1 ;
	Session *sessions;
	Session *s;
	
	sessions = calloc(MAXSESSIONS, sizeof(sizeof(Session)));
	
	
	FILE* file;

	for(;;) {
		
		
		
		alen = sizeof(cli_addr);
		memset((void *)&in,'\0',sizeof(in));

		// Daten vom Socket lesen
		n = recvfrom(sockfd, in, MAXLINE, 0, (struct sockaddr *)&cli_addr, &alen);
		if( n<0 ) {
			err_abort("Fehler beim Lesen des Sockets!");	
		}
		
		
		strcpy(tmp, in);
		
		tok = strtok(tmp,";");
		
		
		if(strncmp(tok,"HSOSSTP_INITX",13)==0){
			
			tok = strtok(NULL,";");
			//neue session
			//if(key = MAXSESSIONS)
			
			sessions[key-1].session_key = key;
			key++;
			
			s = &sessions[key];
			
			s->chunksize = atoi(tok);
			s->chunk = 0;
			
			tok = strtok(NULL,";");
			
			s->filename = calloc(MAXLINE,sizeof(char));
			strcpy(s->filename, tok);
			
			s->file = fopen(s->filename, "r");
			
			
			if(s->file != NULL){
				sprintf(out,"HSOSSTP_SIDXX;%d",s->session_key);
			} 
			else{
				sprintf(out,"HSOSSTP_ERROR;reason; Datei nicht gefunden oder anderer Fehler :( SID: %d Filename: %s",s->session_key, s->filename);
			}
		}
		
		else if(strncmp(tok,"HSOSSTP_GETXX",13)==0){
			
			int i =0;
			
			tok = strtok(NULL , ";");
			
			for(i=0;i<MAXSESSIONS;i++){
				if(sessions[i].session_key = atoi(tok)){
					s = &sessions[i];
				}
			}
			
			FILE * f = s->file;
			read = fread(msg, 1,s->chunksize, f);
			
			if(read<=0){
				sprintf(out,"HSOSSTP_FINXX;chunkno=%d;key=%d;read=%d;msg=%s",s->chunk, s->session_key,read,msg);
				
			}
			else{
				sprintf(out,"HSOSSTP_DATAX;%d;%d;%s",s->chunk,read,msg);
				s->chunk++;
			}
			
		}
		else{
			sprintf(out,"Hier kommen die Daten doch nicht :|");
		}
		
		n = strlen(out);
		

		// Daten schreiben
		if(sendto(sockfd, out, n, 0, (struct sockaddr *)&cli_addr, alen)!=n){
			err_abort("Fehler beim Schreiben des Sockets!");
		}
    }
    
	for(n=0;n<MAXSESSIONS;n++){
		free(sessions[n].filename);
	}	
	free(sessions);
    
    
}


/*
Ausgabe von Fehlermeldungen
*/
void err_abort(char *str){
	fprintf(stderr,"UDP Echo-Server: %s\n",str);
	fflush(stdout);
	fflush(stderr);
	exit(1);
}
