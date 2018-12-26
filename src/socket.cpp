#include "socket.hpp"

Socket::Socket(void)
{
    opt = 1;
    sockfd = 0;
}

Socket::Socket(int fd)
{
    opt = 1;
    sockfd = fd;
}

Socket::~Socket(void)
{

}

int Socket::initSocket(void)
{
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == 0) 
        return SOCKET_CREATION_FAILED;

    if((setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR | 
        SO_REUSEPORT, &opt, sizeof(opt)))) 
        return SOCKET_INIT_FAILED;

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( DEFAULT_PORT );

    if (bind(sockfd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
        return SOCKET_BIND_FAILED;

    return SUCCESS;
}


int Socket::createServer(void)
{
    int errcode = 0;
    if(errcode = initSocket() != 0)
        return errcode;
}

int Socket::process_request(void)
{
    int new_socket;
    if ((new_socket = accept(sockfd, (struct sockaddr *)&address,  
                           (socklen_t*)&addrlen))<0)
    {
        perror("Start server failed");
        return SOCKET_ACCEPT_FAILED;
    }
    return new_socket;

}

int Socket::startServer(void)
{
    
    if(listen(sockfd, 3) < 0)
        return SOCKET_LISTEN_FAILED;
    
        std::cout <<"Listen"<<std::endl;
        
    return SUCCESS;
}


int Socket::createClient(int socket_family, int socket_type, int protocol,
    const char *ipaddr, int port)
{

    std::cout << socket_family<<" Socket family"<<std::endl;
    struct sockaddr_in serv_addr; 
    serv_addr.sin_family = socket_family; 
    serv_addr.sin_port = htons(port);

    if ((sockfd = socket(socket_family, socket_type, protocol)) < 0) 
        return SOCKET_CREATION_FAILED; 

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    
    if(inet_pton(socket_family, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        perror("Create client failed");
        return SOCKET_INVADD_FAILED;
    }

    std::cout << ipaddr << " | "<< port << std::endl;
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Create client failed");
        return SOCKET_CONN_FAILED;
    } 

    std::cout <<"CLIENT "<<std::endl;
    return SUCCESS; 
}

int Socket::createClient(void)
{
    struct sockaddr_in serv_addr; 
    /*int err; 
    const char *ipaddr = "127.0.0.1";
    err = createClient(AF_INET, SOCK_STREAM, 0, ipaddr, DEFAULT_PORT);
    std::cout <<"CLIENT1 "<<err<<std::endl;
    */
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(DEFAULT_PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
  
}

int Socket::socketSend(const void *buf, size_t len)
{
    int valread = 0;   
    send(sockfd , buf , len , 0 );   
}

int Socket::socketRead(void *buf, size_t len)
{
    int valread = 0;
    valread = read( sockfd , buf, len); 
    return valread;
}

int Socket::socketSend(int fd,const void *buf, size_t len)
{
    int valread = 0;   
    send(fd , buf , len , 0 );    
}

int Socket::socketRead(int fd, void *buf, size_t len)
{
    int valread = 0;
    valread = read(fd , buf, len); 
}


int Socket::sendFile(FILE *file)
{
/*
    Send a file via tcp
*/
    int filesize = 0;
    char Sbuf[BUFFERSIZE];
    int nbr = 0;
    int sum = 0;

    fseek(file,0,SEEK_END);
    filesize = ftell(file);
    fseek(file,0,SEEK_SET);

    /*
    *   Send information about file
    */

    nbr = send(sockfd, &filesize,sizeof(filesize), 0);
    if( nbr < sizeof(filesize))
    {
        perror("Send file information ");
        exit(-1);
    }

    nbr = read(sockfd , Sbuf, BUFFERSIZE);
    if(nbr < BUFFERSIZE )
    {
        perror("Send file information ");
        exit(-1);
    }
    

    while(!feof(file))
    {
        nbr = fread(Sbuf, sizeof(char), sizeof(Sbuf), file);
        if( (nbr = send(sockfd,Sbuf,nbr,0)) < 0 )
        {
            perror("Send ");
            return -1;
        }
        sum +=nbr;
        std::cout << "NBR : "<<nbr <<" Total : "<<sum<< std::endl;
    }
    std::cout << "Send "<<sum<<" bytes"<<std::endl;


}

int Socket::recvCMD()
{
    int buf;
    int valread = read( sockfd , &buf, sizeof(int));
    std::cout << buf << "< "<<std::endl;
    return buf;
}

void Socket::sendCMD(int cmd)
{
    int valsend = write( sockfd , &cmd, sizeof(int));
}


int Socket::recvFile(char **buf)
{
    int valread = 0;
    int filesize = 0;
    int sum = 0;
    int nbr = 0;
    char Sbuf[BUFFERSIZE];
    valread = read( sockfd , Sbuf, BUFFERSIZE);
    filesize = *(int *)Sbuf;
    std::cout << "Starting receiving file \n Filesize : "<<filesize<<"\t"<<valread <<std::endl;

    *buf = new char[filesize];
    //std::cout << "recv | " << (void *) buf << std::endl;
    //char *buf = (char *)malloc(filesize * sizeof(char));
    /* Send something to let the other side know we are ready */
    nbr = send(sockfd,Sbuf,BUFFERSIZE,0);
    if( (nbr = send(sockfd,Sbuf,nbr,0)) < 0 )
    {
            perror("Receive send ready : ");
            exit (-1);
    }
    while(sum < filesize)
    {
        nbr = read(sockfd , Sbuf, BUFFERSIZE);
        if(nbr < 0 )
        {
            perror("File read : ");
            exit(-1);
        }
        std::memcpy((*buf)+sum, Sbuf, nbr);
        sum += nbr;
//        std::cout << "NBR : "<<nbr <<" Total : "<<sum<< " [filesize] "<<filesize<<std::endl;
        if(sum > filesize)
        {
            perror("File read : ");
            exit(-1);
        }

    }
    return sum;

}

