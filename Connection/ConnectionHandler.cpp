/* A simple server in the internet domain using TCP
 The port number is passed as an argument
 This version runs forever, forking off a separate
 process for each connection
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>	/* for the waitpid() system call */
#include <signal.h>	/* signal name macros, and the kill() prototype */
#include <string>
#include "../Response/ResponseGenerator.h"
#include <iostream>

using namespace std;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void dostuff(int); /* function prototype */
void error(const string& msg)
{
	perror(msg.c_str());
    exit(1);
}

void server_run(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sigaction sa;          // for signal SIGCHLD
    
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    
    listen(sockfd,5);
    
    clilen = sizeof(cli_addr);
    
    /****** Kill Zombie Processes ******/
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    /*********************************/
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if (newsockfd < 0)
            error("ERROR on accept");
        
        pid = fork(); //create a new process
        if (pid < 0)
            error("ERROR on fork");
        
        if (pid == 0)  { // fork() returns a value of 0 to the child process
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else //returns the process ID of the child process to the parent
            close(newsockfd); // parent doesn't need this
    } /* end of while */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  
 It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
    int n;
    char buffer[256];
    string *message = new string();
//    bzero(buffer,256);
//    n = read(sock,buffer,255);
//    if (n < 0) error("ERROR reading from socket");
//    printf("Here is the message: %s\n",buffer);
//    n = write(sock,"I got your message",18);
//    if (n < 0) error("ERROR writing to socket");
    while (1) {
        bzero(buffer, 256);
        n = read(sock, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        string *segment = new string(buffer);
        message->append(*segment);
        delete segment;
		int msgSize = message->length();
		if (msgSize < 4) {
			break;
		} else {
			if ((*message)[msgSize - 1] == '\n' &&
			    (*message)[msgSize - 2] == '\r' &&
				(*message)[msgSize - 3] == '\n' &&
				(*message)[msgSize - 4] == '\r')
			{
				break;
			}
		}
	}
    cout << *message << endl;
	ResponseGenerator *rg = new ResponseGenerator(*message);
	// string response = *getRespondFromRequest(message);
	// string response = "I've received your message!\n";
	string response = rg->get_status_and_header();
	const char *sendBuffer = response.c_str();
	int sendBufferLength = response.length();
	n = write(sock, sendBuffer, sendBufferLength);
	if (n < 0) error("ERROR writing to socket");
	close(sock);
}

