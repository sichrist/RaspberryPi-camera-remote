#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <thread>
#include "socket.hpp"
#include <iostream>

using namespace std;

class Client
{

public:
	Client(void);
	~Client(void);
	void start(void);

private:
    void ThreadMain(void);
    Socket socket;
    std::thread worker;
    volatile bool stop_thread;
};

#endif