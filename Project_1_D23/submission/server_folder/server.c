#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#define MAXPENDING 5 /* Maximum outstanding connection requests */
#define BUFFER_SIZE 1024
void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket); /* TCP client handling function */

#include <sys/stat.h>
#include <stdbool.h>




int main(int argc, char *argv[])
{ int servSock; /*Socket descriptor for server */
int clntSock; /* Socket descriptor for client */
struct sockaddr_in echoServAddr; /* Local address */
struct sockaddr_in echoClntAddr; /* Client address */
unsigned short echoServPort; /* Server port */
unsigned int clntLen; /* Length of client address data structure */
if (argc != 2) /* Test for correct number of arguments */
{
fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
fflush(stdout);
exit(1);
}
//printf("we haeven't done anything\n");
//fflush(stdout);


echoServPort = atoi(argv[1]); /* First arg: local port */
/* Create socket for incoming connections */
if ((servSock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
DieWithError("socket() failed");
}
//printf("past socket\n");
//fflush(stdout);


/* Construct local address structure */
memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
echoServAddr.sin_family = AF_INET; /* Internet address family */
echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
echoServAddr.sin_port = htons(echoServPort); /* Local port */
/* Bind to the local address */
if (bind (servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr))<0) {
DieWithError("bind() failed");
}
//printf("we made it past bind\n");
//fflush(stdout);


/* Mark the socket so it will listen for incoming connections */
if (listen (servSock, MAXPENDING) < 0) {
DieWithError("listen() failed");
}
//printf("we are going into for loop\n");
//fflush(stdout);


for (;;) /* Run forever */
{
/* Set the size of the in-out parameter */
clntLen = sizeof(echoClntAddr); /* Wait for a client to connect */
if ((clntSock = accept (servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
   DieWithError("accept() failed");
}


/* clntSock is connected to a client! */
printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
fflush(stdout);
//fflush(stdout);
HandleTCPClient(clntSock);
//fflush(stdout);//here?????
}


}
void DieWithError(char *errorMessage)
{
fprintf(stdout,"%s",errorMessage);
fflush(stdout);
//fflush(stdout);


}
void HandleTCPClient(int clntSocket) /* TCP client handling function */
{
char buf[BUFFER_SIZE];
   FILE *tmdg;
   FILE* client = fdopen(clntSocket, "r+b");//read
   FILE* clientWrite = fdopen(clntSocket, "w+b");//read

	int failedToOpen = 0;
  
   //FILE* clientWrite = fdopen(clntSocket, "w+b");//write
   char* message;
   char request[50];
   char *path=(char *)malloc(100*sizeof(char));
   char type[50];
   char request2[50];
   char host[50];
   char newline[50];
   fscanf(client,"%s %s %s\r\n",request, path, type);
   fscanf(client,"%s %s",request2,host);
   

	struct stat file_req_stats;
	char actualPath[50] = ".";
	
  if (strcmp(request, "GET")!=0) {
       fprintf(clientWrite,"method not allowed\r\n\r\n");
       //fflush(stdout);
  }
  else if (strcmp(type, "HTTP/1.1")!=0) {
       fprintf(clientWrite,"HTTP version not supported\r\n\r\n");
       //fflush(stdout);
  }
  else if ((tmdg=fopen(strcat(actualPath,path),"r"))==NULL) { //path is ./TMDG.html
       fprintf(clientWrite,"HTTP/1.1 404 Not Found\r\n\r\n");
	   failedToOpen = 1;
       //fflush(stdout);
  }
	else {

    	fprintf(clientWrite,"HTTP/1.1 200 OK\r\n"); // I believe my fprintf doesn't work
		fstat(fileno(tmdg), &file_req_stats);
    	fprintf(clientWrite, "Content-Length: %d\r\n", (int)file_req_stats.st_size);
    	fprintf(clientWrite, "Content-Type: text/html\r\n\r\n");


      
    	char * line = NULL;
    	size_t len = 0;
       
    	while (getline(&line,&len,tmdg)!=-1) {
        	fprintf(clientWrite,"%s",line);
        	if (strstr(line,"</html>")!=NULL) {
            	break;
        	}
    	}
   }
   fclose(clientWrite);
   if (failedToOpen!=1) {
	fclose(tmdg);
   } 
   
   fclose(client);
   
}
/* NOT REACHED */

