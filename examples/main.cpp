#include "socket.hpp"
#include "client.hpp"
#include "server.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[])
{
	
	Server server;
	server.start();

	return 0;
}