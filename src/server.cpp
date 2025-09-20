#include "/Users/erkhembilegariunbold/CLionProjects/myRedis/include/server.h"

#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

static Server* globalServer = nullptr;

Server::Server(int port) {
    this->port = port;
    this->serverSocket = -1;
    this->running = true;
    globalServer = this;
}
// Building the server
void Server::run() {  // different hosts, TCP, protocol
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);  // creating a TCP connection socket
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }
    int option = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(port); // port in network byte order
    serverAddr.sin_addr.s_addr = INADDR_ANY; // listen on all network interfaces
    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return;
    }
    if (listen(serverSocket, 10) < 0) {
        std::cout << "Error in listening on server socket" << std::endl;
        return;
    } else {
        std::cout << "Listening on port " << this->port << std::endl;
    }
}

// shut down
void Server::stop() {
    this->running = false;
    if (serverSocket != -1) {
        close(serverSocket);  // closes the listening socket
    }
    std::cout << "Server stopped" << std::endl;
}