#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;
std::string 

class SocketThread : public Thread      // client connection
{
private:
    Socket& socket;     // this is the connected socket
    ByteArray data;     // data
public:
    SocketThread(Socket& socket)
    : socket(socket)
    {}
    ~SocketThread()
    {}
    Socket& GetSocket() {
        return socket;
    }
    virtual long ThreadMain() {
        while(1) {
            try {
                socket.Read(data);
                std:cout << "Reading Data." << std::endl;
                std::string st = data.ToString();
                if (st == "Y") {
                    std::cout << "Socket has been termninated. Press 'Enter' to clsoe the server" << std::endl;
                    break;
                }
                std::reverse(st.begin(), st.end());
                data = ByteArray(st);
                std::cout << "Sending the data" << st << std::endl;
                socket.Write(data);
                std::cout << "Data has been sent" << std::endl;
            }
            catch(...){}
        }
        return 0;
    }
}

// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    std::vector<SocketThread*> socketThreadVector;
    bool terminate = false;
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Cleanup
	//...
        for (int x = 0; x < socketThreadVector.size(); x++) {
            Socket&  socket = socketThreadVector[x]->GetSocket();
            socket.Close();
        }
    }

    virtual long ThreadMain()
    {
        while(true) {
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());

        // A reference to this pointer 
        Socket& socketReference = *newConnection;
	//You can use this to read data from socket and write data to socket. You may want to put this read/write somewhere else. You may use ByteArray
	// Wait for data
        //socketReference.Read(data);
        // Send it back
        //socketReference.Write(data);
        socketThreadVector.push_back(new SocketThread(socketReference));
        }
    }
};


int main(void)
{
    std::cout << "I am a server." << std::endl;
	
    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);
	
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    // Shut down and clean up the server
    server.Shutdown();

}
