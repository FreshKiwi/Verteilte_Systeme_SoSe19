/* 
 Beispiel TCP Echo-Server fuer mehrere Clients 
 Aus Stevens: Unix Network Programming 
 CWe 10/2016: adaptiert fuer Ubuntu 16.04 64Bit
*/

#include <unistd.h>    // fuer read, write etc.
#include <stdlib.h>     // fuer exit
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>

#define SRV_PORT 8998
#define MAX_SOCK 10
#define MAXLINE 512
#define CHUNKSIZE 100

// Vorwaertsdeklarationen intern
void resp(int, DIR *); 
void err_abort(char *str);
void getFileName(char *, char **);
int createHeader(char *, char *);

int main(int argc, char *argv[]) {
	char *name;
	char docroot [MAXLINE],cwd [MAXLINE];
	long int port;
	
	DIR *dir;   
    
    
	//Behandeln der Argumente
	strcpy(docroot, argv[1]);
        
    getcwd(cwd, sizeof(cwd));
    printf("cwd = %s\n",cwd);
        
    if ((chdir (docroot)) != 0) {
        printf("ERROR: %s\n", strerror(errno));
        exit (1);
    }
    
    
    port = strtol(argv[2],NULL,10);
    
    if( port == 0 ){
		printf("Syntaxfehelr: Port muss Integer sein!\n");
		exit (1);
	} 
	
	// Deskriptoren, Adresslaenge, Prozess-ID 
	int sockfd, newsockfd, alen, pid;
	int reuse = 1;

	// Socket Adressen
	struct sockaddr_in cli_addr, srv_addr;

	// TCP-Socket erzeugen
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_abort("Kann Stream-Socket nicht oeffnen!");
	}

	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0){
		err_abort("Kann Socketoption nicht setzen!");
	}

	// Binden der lokalen Adresse damit Clients uns erreichen
	memset((void *)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(port);
	if( bind(sockfd, (struct sockaddr *)&srv_addr,
		sizeof(srv_addr)) < 0 ) {
		err_abort("Kann lokale Adresse nicht binden, laeuft fremder Server?");
	}

	// Warteschlange fuer TCP-Socket einrichten
	listen(sockfd,5);
	printf("TCP Echo-Server: bereit ...\n");

	for(;;){
                printf("_1___________________for(;;)[Main]\n");
		alen = sizeof(cli_addr);

		// Verbindung aufbauen
		newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&alen);
                
		if(newsockfd < 0){
			err_abort("Fehler beim Verbindungsaufbau!");
		}
                else{

		// fuer jede Verbindung einen Kindprozess erzeugen
		if((pid = fork()) < 0){
			err_abort("Fehler beim Erzeugen eines Kindprozesses!");
		}else if(pid == 0){
                        printf("_2___________________pid==0\n");
			close(sockfd);
			resp(newsockfd, dir);
			exit(0);
		}
                }
               printf("_3___________________close(newsockfd)\n");
		close(newsockfd);
	}
    printf("_4___________________Fin.\n");
} 
/* str_echo: Lesen von Daten vom Socket und an den Client zuruecksenden 
 */
void resp(int sockfd, DIR * dir) {
    FILE * file;
    int n, msg_length, resp_choice;
    char in[MAXLINE], out[MAXLINE], msg[MAXLINE],filename[MAXLINE];

    memset((void *) in, '\0', MAXLINE);

    // Daten vom Socket lesen
    n = read(sockfd, in, MAXLINE);
    printf("_5___________________in:\n %s\n", in);
    if (n == 0) {
        return;
    } else if (n < 0) {
        err_abort("Fehler beim Lesen des Sockets!");
    }

    //Filename extrahieren
    getFileName(in, &filename);
    printf("_6.1___________________filename: %s\n",filename);
    if(!strncmp(filename,"ServerHTTPPost",14)){
        file = fopen("response.html", "r");
    }
    else{
        file = fopen(filename, "r");
        printf("_6.1___________________filename: %s\n",filename);
    }
    
    if (file) {

        msg_length = CHUNKSIZE;

        //Senden der Response
        resp_choice = createHeader(out,in);
        
        n = strlen(out);
        if (write(sockfd, out, n) != n)  {
            err_abort("Fehler beim Schreiben des Sockets!");
        }

        if (resp_choice == 0) {
            while (msg_length == CHUNKSIZE) {

                msg_length = fread(out, 1, CHUNKSIZE, file);

                out[msg_length] = '\0';
                n = strlen(out);

                if (write(sockfd, out, n) != n) {
                    err_abort("Fehler beim Schreiben des Sockets!");
                }

            }
        }
        else if(resp_choice == 1){
            while (msg_length == CHUNKSIZE) {

                msg_length = fread(out, 1, CHUNKSIZE, file);

                n = msg_length;

                if (write(sockfd, out, n) != n) {
                    err_abort("Fehler beim Schreiben des Sockets!");
                }

            }
        }

        printf("WHILE wurde durchbrochen, ms_lngth= %d\n", msg_length);
        fclose(file);
    } else {

        sprintf(out, "HTTP/1.1 404 NOT_FOUND");
        n = strlen(out);
        if (write(sockfd, out, n) != n) {
            err_abort("Fehler beim Schreiben des Sockets!");
        }
    }
}


int createHeader(char *out, char *in){
    if(strstr(in,"text/html")){    
        char  header [MAXLINE] ="HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\n\n";
        sprintf(out,"%s", header);
        return 0;
    }
    if(strstr(in,"image/webp")){    
        char  header [MAXLINE] ="HTTP/1.1 200 OK\nContent-Type: image/webp; charset=utf-8\n\n";
        sprintf(out,"%s", header);
        return 1;
    }
    else return 0;
}

void getFileName(char * req, char** filename) {
    char tmp[MAXLINE];
    if(strstr(req,"GET")){
        sscanf (req, "GET /%255s HTTP/", tmp);
        strcpy(filename, tmp);
    }
    else{ 
        sscanf (req, "POST /%255s HTTP/", tmp);
        strcpy(filename, tmp);
    }
}


/*
Ausgabe von Fehlermeldungen
*/
void err_abort(char *str){
	fprintf(stderr," TCP Echo-Server: %s\n",str);
	fflush(stdout);
	fflush(stderr);
	exit(1);
}
