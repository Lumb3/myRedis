#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <string>
#include <vector>
#include <unordered_map>
class CommandHandler {
public:
    CommandHandler();
    std::vector<std::string> parseRespCommand(const std::string& input);
     std::string process (const std::string& commandLine);
private:
    std::unordered_map<std::string, std::string> db; // in-memory key-value store
};
#endif