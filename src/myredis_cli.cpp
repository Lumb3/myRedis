#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 6379;

    if (argc >= 2) {
        port = std::stoi(argv[1]);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0); // Creates a new socket
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    std::cout << "Connected to myRedis on port " << port << std::endl;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "quit") break;

        // Convert input to RESP
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) tokens.emplace_back(token);

        if (tokens.empty()) continue;

        std::string resp = "*" + std::to_string(tokens.size()) + "\r\n";
        for (auto& t : tokens) {
            resp += "$" + std::to_string(t.size()) + "\r\n" + t + "\r\n";
        }

        send(sock, resp.c_str(), resp.size(), 0);

        char buffer[1024];
        ssize_t bytes = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            std::cout << "Disconnected from server." << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        std::cout << buffer;
    }

    close(sock);
    return 0;
}
