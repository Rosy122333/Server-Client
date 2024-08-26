#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#define RCVBUFSIZE 32 /* Size of receive buffer */
void DieWithError(char *errorMessage); /* Error handling function */ 

#include <sys/types.h> /*I put this here*/
#include <netdb.h>  /*I put this here*/ 
#include <netinet/in.h>  /* this includes the socket address structure*/
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/resource.h>
int wasThereError;

#define BUFFER_SIZE 1024
int main(int argc, char *argv[])
{
wasThereError = 0;
int serverAddress; /* Socket descriptor */
struct sockaddr_in echoServAddr; /* Echo server address */
unsigned short echoServPort; /* Echo server port */
char *servIP; /* Server IP address (dotted quad) */
int sock;
//added in
char *servURL;
char *option = NULL;
unsigned short portNUM;
const char *charPortNUM;
int error;
char str[50];

struct addrinfo *results;
int status;
int clientSocket;

char *echoString; /* String to send to echo server */
char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
unsigned int echoStringLen; /* Length of string to echo */
int bytesRcvd, totalBytesRcvd; /* Bytes read in single recv()
and total bytes read */

if ((argc < 3) || (argc > 4)) /* Test for correct number of arguments */
{
fprintf(stderr, "Usage: %s [-options] <server_url> <port_number>\n",argv[0]);
exit(1);
} 
if (argc == 3) {
servURL = argv[1]; /* First arg: server URL */
portNUM = atoi(argv[2]); /* Second arg: port number */
charPortNUM = argv[2];
} else if (argc == 4) {
    //argv[1] will be the -p
    option = argv[1];
    servURL = argv[2];
    portNUM = atoi(argv[3]); /* Use given port, if any */
    charPortNUM = argv[3];
}


//ask about this, parsing now
char path[50];
char *src = NULL;
char *host = NULL;
int givenTDGM = -1;
char * token2 = strtok(servURL,"/");//HERERHEHRHEHRHRE
servURL = token2;
while (token2!=NULL) {
    givenTDGM++;
    if (givenTDGM==1) {
        src = token2;
        strcpy(path,"/");
        //path = "/";
        //adding a slash in front of the path
        strcat(path,src);
    }
    token2 = strtok(NULL,"/");
}
host = servURL;



//printf("Here is the URL %s here is the path %s\n",host,path);
//fflush(stdout);








/* Create a reliable, stream socket using TCP */
if ((sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
} 



/* Construct the server address info struct */
struct addrinfo hints;
memset(&hints, 0, sizeof(struct addrinfo)); /* Zero out structure */
hints.ai_family = AF_INET; /* Internet address family */
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
//added this in here

if ((serverAddress = getaddrinfo(host, charPortNUM, &hints, &results))!=0) { //host
    //printf("this is the servURL %s",servURL);
    DieWithError("getaddrinfo() failed");
}

struct timespec start, finish;
clock_gettime( CLOCK_REALTIME, &start );

if (connect(sock,results->ai_addr,results->ai_addrlen) == -1) {
    DieWithError("connect failed");
}


char buffer[BUFFER_SIZE];


FILE* ptr = fdopen(sock, "r+b");//read plus 
if (NULL == ptr) {
        printf("file can't be opened \n");
}
//www.google.com
if (wasThereError ==0) {
    printf("Server Response \n");
    printf("-------------------------\n");
}



if (givenTDGM>=1) {
    fprintf(ptr, "GET %s HTTP/1.1\r\n", path);//path ----edit this
} else {
    fprintf(ptr, "GET %s HTTP/1.1\r\n", "/");//path ----edit this
}
 
fprintf(ptr, "Host: %s\r\n\r\n",host); //host



char* token;
bool keepreading = true;
int count =0;
char * line = NULL;
size_t len = 0;
int CLval = 0;

//do I have to change anything in here??


while(true) {
    if (getline(&line,&len,ptr)==-1) {
        fprintf(stderr, "getline failed while doing header");
        fflush(stderr);
        exit(-1);
    }
    if (strcmp("\r\n", line)==0) {
        fprintf(stdout, "%s", line);
        fflush(stdout);
        break;
    }
    if (strstr(line,"404 Not Found")) {
        fprintf(stdout, "%s", line);
        fflush(stdout);
        exit(0);
    }
    if (strstr(line,"Content-Length: ")!=NULL) {
        token = strtok(line,"Content-Length: ");
        CLval = atoi(token);
        fflush(stdout);

    }
    fprintf(stdout, "%s", line);
    fflush(stdout);
}

clock_gettime( CLOCK_REALTIME, &finish);

FILE *out_file;

    out_file = fopen("index.html", "w");



while (true) {
    count++;
    if (getline(&line,&len,ptr)==-1) {
        fprintf(stderr, " getline failed while doing body");
        fflush(stderr);
        exit(-1);
        break;
    }
    if (strstr(line,"</html>")!=NULL) {
        fprintf(out_file,"%s",line);
        break;
    }
    fprintf(out_file,"%s",line);
}
fflush(stdout);


fprintf(stdout,"Page Saved to \"index.html\" \n");


fflush(stdout);

if (argc==4) {
    if (strstr(option,"-p")!=NULL) {
    printf( "The return trip time is %f in milliseconds\n", ((double) (finish.tv_nsec - start.tv_nsec))/((double) 1000000) );
    fflush(stdout);
    }
}
fclose(ptr); //listen afterwards??
fclose(out_file);




// echoStringLen = strlen(echoString); /* Determine input length */
// /* Send the string to the server */
// if (send (sock, echoString, echoStringLen, 0) != echoStringLen)
//     DieWithError("send() sent a different number of bytes than expected");
// /* Receive the same string back from the server */
// totalBytesRcvd = 0;
// printf("Received: "); /* Setup to print the echoed string */ 

// while (totalBytesRcvd < echoStringLen)
// {
// /* Receive up to the buffer si ze (minus 1 to leave space for
//  * a null terminator) bytes from the sender */
// if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
//     DieWithError("recv() failed or connection closed prematurely");
// totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
// echoBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
// printf("%s", echoBuffer); /* Print the echo buffer */
// }
// printf("\n"); /* Print a final linefeed */


close (sock);
exit(0);
}

void DieWithError(char *errorMessage)
{
        printf("%s",errorMessage);
        printf("\n");
        wasThereError++;
} 














