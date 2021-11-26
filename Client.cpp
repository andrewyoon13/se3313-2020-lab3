
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

bool flag = true;

class ClientThread : public Thread {
	private:
		Socket& socket;

		ByteArray data;
		std::string data_str;
	public:
		ClientThread (Socket& socket): socket(socket){}
		~ClientThread(){}

		virtual long ThreadMain() {
			bool trigger = false;
			try {
				int result = socket.Open();
			}
			catch (...) {
				std::cout << "Run Server" << std::endl;
				trigger = true;
				flag = false;
			}
			while (1) {
				if (trigger) {
					break;
				}
				std::cout << "Enter Data:";
				std::cout.flush();
				std::getline(std::cin, data_str);	// Receives that entered data
				data = ByteArray(data_str);
				socket.Write(data);					// Writes that data to the server
				if (data_str == "Y") {
					flag = false;
					break;
				}
				socket.Read(data);					// Get response from the server
				data_str = data.ToString();
				std::cout << "Server Response: " << data_str << std:endl;
			}
			std::cout << "Closing." << std::endl;
			return 0;
		}
};
int main(void)
{
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	//To write to socket and read from socket. You may use ByteArray 
	//socket.Write(data);
	//socket.Read(data);
	ClientThread clientThread(socket);
	while(flag) {
		sleep(1);
	}
	try {
		socket.Close();
		std::cout << "Socket Closed" << std::endl;
	}
	catch(...) {
		std::cout << "Closing" << std::endl;
	}
	return 0;
}
