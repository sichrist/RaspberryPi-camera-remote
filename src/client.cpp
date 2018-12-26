#include "client.hpp"
#include "cmd.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
Client::Client(void) : worker()
{

}


Client::~Client(void)
{
	stop_thread = true;
	if(worker.joinable()) worker.join();
}




void Client::ThreadMain(void)
{

	socket.createClient();
	cout << "Client"<<endl;

	/*start new process with*/
	socket.sendCMD(CMD_PROCESS);
	char Sbuf[4096];
	int n = 0;

	Sbuf[0] = '\0';
	// get image information
	n = socket.socketRead(Sbuf,4096);
	std::string inf(Sbuf);
	std::cout << inf << std::endl;

	/// parsing form image size
	std::vector<int> v;
	int nbr = 0;
	size_t pos = 0;
	std::string token;
	std::string delimiter = ",";

	while( (pos = inf.find(delimiter)) != std::string::npos)
	{
		token = inf.substr(0,pos);
		nbr = std::stoi(token);
		std::cout << nbr << std::endl;
		v.push_back(nbr);
		inf.erase(0,pos + delimiter.length());
		
	}
	std::cout << inf << std::endl;
	nbr = std::stoi(inf);
	v.push_back(nbr);
	
	//----------------------------------------------------------

	int x = v[0];
	int y = v[1];
	char imgBuf[v[2]];

	// Ask for image

	socket.sendCMD(CMD_OK);

	n = 0;
	int total = 0;

	// Wait untill the whole image is received
	while(total < v[2])
	{
		n = socket.socketRead(&imgBuf[n],v[2]-n);
		total +=n;
	}


	// Convert received information to image
  	std::vector<unsigned char> vectordata(imgBuf,imgBuf+v[2]);
  	cv::Mat data_mat(vectordata,true);
	cv::Mat image(cv::imdecode(data_mat,1));
	cout<<"Height: " << image.rows <<" Width: "<<image.cols<<endl;
  	cv::imwrite("out2.jpg", image);

}

void Client::start(void)
{
	worker = std::thread(&Client::ThreadMain,this);
}