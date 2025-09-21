#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H
#include <atomic>
#include <string>

class Server {
public:
    Server(int port);
    void run();
    void stop();
    void dumpDatabase();
private:
    int port;
    int serverSocket;
    std::atomic<bool> running;
};
#endif