#include <iostream>
#include "../myRedis/include/server.h"
#include <thread>
#include <chrono>
#include "../myRedis/include/command_handler.h"

int main (int argc, char * argv[]) { // argument count, argument vector (arr of C-style string that holds the arguments)
    int port = 6379; // default port
    if (argc >= 2) {
        port = std::stoi(argv[1]); // ./myredis 8000 (start the server at 8000 port)
    }
    Server server(port);

    // dump the databases
    std::thread dumpThread([&server] () {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5)); // shorter for demo
            std::cout << "Dumping database..." << std::endl;
            // server.dumpDatabase();
        }
    });

    dumpThread.detach();  // let the thread run in the background

    server.run();
    return 0;
}