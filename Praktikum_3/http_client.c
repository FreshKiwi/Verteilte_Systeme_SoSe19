/*
 Beispiel: TCP Echo-Client 
 Stevens, R., Fenner, B., Rudoff, A. M.: Unix Network Programming 
 CWe 10/2016:  adaptiert fuer Ubuntu 16.04 64Bit
*/

#include <unistd.h>    // fuer read, write etc.
#include <stdlib.h>     // fuer exit
#include <stdio.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SRV_PORT 8998
#define MAXLINE 512

void str_client(int);
void err_abort(char *str);

int main(int argc, char *argv[]) {
	// Deskriptor
	int sockfd;
	// Socket Adresse
	struct sockaddr_in srv_addr, cli_addr;

	// Argumente testen
	if( argc != 2 ) {
		err_abort("IP Adresse des Servers fehlt!");
	}

	// TCP Socket erzeugen
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_abort("Kann Stream-Socket nicht oeffnen!");
	}

	// Adress Struktur fuer Server aufbauen
	memset((void *)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	srv_addr.sin_port = htons(SRV_PORT);

	// Verbindung aufbauen
	if(connect(sockfd,(struct sockaddr *)&srv_addr,sizeof(srv_addr)) < 0){
		err_abort("Fehler beim Verbindungsaufbau!");
	}
	printf("TCP Echo-Client: bereit...\n");

	// Daten zum Server senden
	str_client(sockfd);

	close(sockfd);
	exit(0);
}

/*
 str_client: Daten von der Standardeingabe lesen, zum Server senden, auf das Echo warten und dieses ausgeben
*/
void str_client(int sockfd){
	int n;
	char out[MAXLINE],in[MAXLINE+6];

	// Lesen bis zum Ende der Eingabe
	while(fgets(out,MAXLINE,stdin)!=NULL){
		n=strlen(out);
		out[n-1]='\0';
		
		// Zeile zum Server senden
		if(write(sockfd,out,n)!=n){
			err_abort("Fehler beim Schreiben des Sockets!");
		}
      
		// Echo vom Server lesen
		n=read(sockfd,in,MAXLINE);
		if(n<0){
			err_abort("Fehler beim Lesen des Sockets!");
		} 

		// ausgeben
		printf("%s\n",in);
	}
}

/*
Ausgabe von fehlern und Beenden des Programms
*/
void err_abort(char *str){
	fprintf(stderr,"TCP Echo-Client: %s\n",str);
	fflush(stdout);
	fflush(stdin);
	exit(1);
}
