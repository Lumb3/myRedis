#include <iostream>
#include <thread>
#include <chrono>
#include "../include/server.h"
#include "../include/command_handler.h"

int main(int argc, char* argv[]) {
    int port = 6379; // default Redis port
    if (argc >= 2) {
        port = std::stoi(argv[1]); // ./myredis 8000 (start on port 8000)
    }

    Server server(port);

    // // Background thread to simulate database dumps
    // std::thread dumpThread([&server]() {
    //     while (true) {
    //         std::this_thread::sleep_for(std::chrono::seconds(5)); // shorter for demo
    //         std::cout << "Dumping database..." << std::endl;
    //         // server.dumpDatabase();
    //     }
    // });
    //
    // dumpThread.detach(); // run independently

    server.run();
    return 0;
}
