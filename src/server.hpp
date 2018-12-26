#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
#include "socket.hpp"
#include <iostream>
#include <sys/wait.h>	// Waitpid

using namespace std;


class Server
{


public:
	Server(void);
	~Server(void);
	void start(void);


private:
	void ThreadMain(void);
    Socket socket;
    std::thread worker;
    volatile bool stop_thread;
};


class Worker
{


public:
	Worker(int fd);
	~Worker();


private:
	void start(void);
	int socket;
	volatile bool stop_thread;
	std::thread worker;
};


#endif