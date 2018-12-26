#include "server.hpp"
#include "cmd.hpp"

Server::Server(void) : worker()
{
    int err = 0;
    err = socket.createServer();
}


Server::~Server(void)
{
    if(worker.joinable()) worker.join();
}


void Server::ThreadMain(void)
{
/*
* Thread for processing request
* When a connection is accepted a worker-thread is started to handle the request
*/

    std::vector<Worker *> workerV;
    Worker *worker = NULL;
    int err = 0;
    int new_socket = 0 ;

    err = socket.startServer();
    cout << "HIER "<< stop_thread<<endl;
    while(! stop_thread )
    {
        new_socket = socket.process_request();
        if(new_socket == SOCKET_ACCEPT_FAILED)
        {
            cout << "Socket SOCKET_ACCEPT_FAILED "<<endl;
        }
        
        cout << "Incomming connection"<<endl;
        workerV.push_back(new Worker(new_socket));
    }
    cout << "HIER"<<endl;
}


void Server::start(void)
{
    worker = std::thread(&Server::ThreadMain,this);
}


Worker::Worker(int fd) : worker()
{
    socket = fd;
    stop_thread = false;
    worker = std::thread(&Worker::start,this);
}


Worker::~Worker()
{
    stop_thread = true;
}


void Worker::start(void)
{


    /*
        Open picture in test/
    */


    //FILE *picture;
    //picture = fopen("/home/simon/Dokumente/RaspberryPi-camera-remote/test/test.jpg","rb");
    Socket socket_(socket);
    int cmd = socket_.recvCMD();

    std::cout<<"found cmd : "<<cmd << std::endl;
    if(cmd == CMD_PROCESS)
    {
        pid_t   parent;
        pid_t   pid;
        parent = getpid();

        pid = fork();

        if(pid == -1)
        {
            std::cerr<<"Failed to fork process"<<std::endl;
        }

        if(pid > 0)
        {
            int status;
            waitpid(pid, &status, 0);
        }
        else
        {
            std::string fd = std::to_string(socket);
            std::cout << "FD : "<<socket <<std::endl;
            execl("src/test.py", "test", fd.c_str(),NULL);
            std::cerr << "execl failed "<<std::endl;
        }
    }
    //socket_.sendFile(picture);
    
}