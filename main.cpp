#include <iostream>
#include "../myRedis/include/server.h"
#include "../myRedis/include/command_handler.h"


int main (int argc, char * argv[]) { // argument count, argument vector (arr of C-style string that holds the arguments)
    int port = 6379; // default port
    if (argc >= 2) {
        port = std::stoi(argv[1]); // ./myredis 8000 (start the server at 8000 port)
    }
    Server server(port);
    server.run();
    return 0;
}