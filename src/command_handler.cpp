#include "../include/command_handler.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// *2\r\n$5\r\nhello\r\n$5\r\nworld\r\n
// *2 means an array with 2 elements
// $5 length
// hello -> token1
// world -> token2

std::vector<std::string> parseRespCommand(const std::string &input) {
    std::vector<std::string> tokens;
    if (input.empty())
        return tokens; // return empty tokens vector
    if (input[0] != '*') {
       std::istringstream ss (input);
        std::string token;
        while (ss >> token) {
            tokens.emplace_back(token);
        }
        return tokens;
    }
    size_t position = 0;
    if (input[position] == '*') {
        ++position;
        size_t count = std::stoul(input.substr(position, input.find("\r\n", position) - position));
        position = input.find("\r\n", position) + 2;

        for (size_t i = 0; i < count; i++) {
            if (input[position] != '$') break;
            ++position;
            size_t len = std::stoul(input.substr (position, input.find("\r\n", position) - position));
            position = input.find("\r\n", position) + 2;
            std::string element = input.substr (position, len);
            tokens.emplace_back(element);
            position += len + 2; // skip \r\n string
        }
    }
    return tokens;
}
CommandHandler:: CommandHandler() {}

std::string CommandHandler::process(const std::string &commandLine) {
    auto tokens = parseRespCommand(commandLine);
    if (tokens.empty()) return "-Error: Empty command\r\n";

    std::string command = tokens[0];
    std::ranges::transform(command, command.begin(), ::toupper);

    std::ostringstream response;

    if (command == "PING") {
        response << "+PONG\r\n";
    }
    else if (command == "ECHO") {
        if (tokens.size() < 2) {
            response << "-Error: ECHO requires an argument\r\n";
        } else {
            response << "+" << tokens[1] << "\r\n";
        }
    }
    else if (command == "SET") {
        if (tokens.size() < 3) {
            response << "-Error: SET requires key and value\r\n";
        } else {
            std::string key = tokens[1];
            std::string value = tokens[2];
            db[key] = value;
            response << "+OK\r\n";
        }
    }
    else if (command == "GET") {
        if (tokens.size() < 2) {
            response << "-Error: GET requires key\r\n";
        } else {
            std::string key = tokens[1];
            if (db.find(key) != db.end()) {
                response << "$" << db[key].size() << "\r\n" << db[key] << "\r\n";
            } else {
                response << "$-1\r\n"; // key not found
            }
        }
    }
    else {
        response << "-Error: Unknown command\r\n";
    }

    return response.str();
}

