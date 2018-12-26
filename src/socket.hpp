#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h> 
#include <arpa/inet.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <vector> 
#include <cstring>
#include  <iomanip>
#define SUCCESS					0
/* 
*    Error codes for socket creation
*/
#define SOCKET_CREATION_FAILED  1000
#define SOCKET_INIT_FAILED 		1001
#define SOCKET_BIND_FAILED 		1002
#define SOCKET_LISTEN_FAILED 	1003
#define SOCKET_ACCEPT_FAILED 	1004
#define SOCKET_INVADD_FAILED 	1005
#define SOCKET_CONN_FAILED		1006

#define DEFAULT_PORT 			8080

#define BUFFERSIZE				4096

class Socket
{
public:
    Socket(void);
    Socket(int fd);
    ~Socket(void);
    int createServer(void);
    int createClient(void);
    void start(void);
    int socketSend(const void *buf, size_t len);
    int socketRead(void *buf, size_t len);
    int socketSend(int fd,const void *buf, size_t len);
    int socketRead(int fd,void *buf, size_t len);
    int startServer(void);
    int startClient(void);
    int process_request(void);
    int sendFile(FILE *file);
    int recvFile(char **buf);
    int recvCMD();
    void sendCMD(int cmd);
    int createClient(int socket_family, int socket_type, int protocol, 
    	const char *ipaddr, int port);

private:
    struct sockaddr_in address; 
    int addrlen;
    int sockfd;
    int opt;   
    int initSocket(void);
    
    
};

#endif