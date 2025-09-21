#include "../include/server.h"
#include "../include/command_handler.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>

static Server* globalServer = nullptr;

Server::Server(int port) {
    this->port = port;
    this->serverSocket = -1;
    this->running = true;
    globalServer = this;
}

Server::~Server() {
    stop(); // ensure resources are cleaned up
}

void Server::run() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);  // TCP socket
    if (serverSocket < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    int option = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    sockaddr_in serverAddr{}; // structure for IPv4 socket address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
        close(serverSocket);
        exit(1);
    }

    std::cout << "Listening on port " << this->port << std::endl;

    while (this->running) {
        int client_socket = accept(serverSocket, nullptr, nullptr);
        if (client_socket < 0) {
            if (running) {
                std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
            }
            continue;
        }

        // Handle each client in its own thread
        std::thread([client_socket]() {
            CommandHandler c; // thread-local handler
            char buffer[1024];
            ssize_t bytes;

            while ((bytes = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytes] = '\0';
                std::string response = c.process(std::string(buffer));
                send(client_socket, response.c_str(), response.size(), 0);
            }

            close(client_socket);
        }).detach();
    }
}

void Server::stop() {
    this->running = false;
    if (serverSocket != -1) {
        shutdown(serverSocket, SHUT_RDWR); // wake accept()
        close(serverSocket);
        serverSocket = -1;
    }
    std::cout << "Server stopped" << std::endl;
}

// Example dump method (disabled for now)
// void Server::dumpDatabase() {
//     std::ofstream out("dump.rdb", std::ios::trunc);
//     if (!out.is_open()) {
//         std::cerr << "Error opening dump file" << std::endl;
//         return;
//     }
//     out << "Dumping database at port " << this->port << std::endl;
//     out.close();
//     std::cout << "Database dumped to dump.rdb" << std::endl;
// }
