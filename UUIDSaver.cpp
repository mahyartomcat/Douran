//#include "Poco/Net/TCPServerConnection.h"
//#include "Poco/Thread.h"
//#include "Poco/RunnableAdapter.h"
//#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"

#include <string>
#include <thread>
#include <memory>
#include <fstream>

#include "uuid_singleton.h"

using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
//using Poco::Net::TCPServerConnection;
//using Poco::Net::TCPServerParams;


class PerThreadConnection
{
public:
	PerThreadConnection(int numReq = 1) : m_numOfReqPerThread(numReq) {}

	void run()
	{
		try
		{
			SocketAddress sa("127.0.0.1", 9911);
			std::unique_ptr<StreamSocket> ss;
			
			for (int i = 0; i<m_numOfReqPerThread; i++)
			{
				char buffer[256];
				ss = std::make_unique<StreamSocket>(StreamSocket(sa));
				int n = ss->receiveBytes(buffer, sizeof(buffer));

				if (n>0)
				{
					UUIDLogger::instance().log(std::string(buffer, n));
				}
				ss->close();
			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "Error in thread connection: " << exc.displayText() << std::endl;
		}
	}

private:
	int m_numOfReqPerThread;
};


int main(int argc, char** argv)
{
	// simple input controlling
	if (argc!=4)
	{
		std::cout << 
			"Please specify number of threads, number of requests from server and name of output file!\n" << 
			 "example: $ uuidsaver 10 1000 uuid.txt\n" << std::endl;
		return 0;
	}

	int numberOfThreads = 1;
	int numberOfRequests = 1;
	int numOfReqPerThread = 1;
	std::string filename = "uuid.txt";

	// FIXME: use more elaborate methods for given inputs.
	//        for example using regex for file name verification
	try{
		numberOfThreads = std::stoi(argv[1]);
		numberOfRequests = std::stoi(argv[2]);
		filename = std::string(argv[3]);

		if (numberOfThreads == 0)
			throw std::exception("Devide by zero");

		// FIXME: if numberOfThreads was not quotient of numberOfRequests
		//		  total requested connections will not execute.
		numOfReqPerThread = numberOfRequests / numberOfThreads;
		if (numOfReqPerThread<1)
			numOfReqPerThread = 1;
	}
	catch (std::exception& exc)
	{
		std::cout << exc.what() << std::endl;
	}

	UUIDLogger::setFileName(filename);


	std::vector<std::thread> threads;
	for (int i = 0; i < numberOfThreads; ++i) 
	{
		PerThreadConnection ptr(numOfReqPerThread);
		threads.push_back(std::thread(&PerThreadConnection::run, &ptr));
	}

	for (auto& t : threads) 
	{
		t.join();
	}

	return 0;
}
