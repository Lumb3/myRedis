#include "../include/command_handler.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Parse RESP input into tokens
std::vector<std::string> CommandHandler::parseRespCommand(const std::string &input) {
    std::vector<std::string> tokens;
    if (input.empty()) return tokens;

    if (input[0] != '*') {
        // Fallback: space-separated command
        std::istringstream ss(input);
        std::string token;
        while (ss >> token) {
            tokens.emplace_back(token);
        }
        return tokens;
    }

    size_t position = 1; // skip '*'
    size_t countEnd = input.find("\r\n", position);
    if (countEnd == std::string::npos) return tokens;

    size_t count = std::stoul(input.substr(position, countEnd - position));
    position = countEnd + 2;

    for (size_t i = 0; i < count; i++) {
        if (input[position] != '$') break;
        ++position;

        size_t lenEnd = input.find("\r\n", position);
        if (lenEnd == std::string::npos) break;

        size_t len = std::stoul(input.substr(position, lenEnd - position));
        position = lenEnd + 2;

        if (position + len > input.size()) break; // malformed input

        std::string element = input.substr(position, len);
        tokens.emplace_back(element);

        position += len + 2; // skip data + \r\n
    }

    return tokens;
}

CommandHandler::CommandHandler() = default;

std::string CommandHandler::process(const std::string &commandLine) {
    auto tokens = parseRespCommand(commandLine);
    if (tokens.empty()) return "-Error: Empty command\r\n";

    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(),
                   [](unsigned char c){ return std::toupper(c); });

    std::ostringstream response;

    if (command == "PING") {
        response << "+PONG\r\n";
    }
    else if (command == "ECHO") {
        if (tokens.size() < 2) {
            response << "-Error: ECHO requires an argument\r\n";
        } else {
            response << "$" << tokens[1].size() << "\r\n" << tokens[1] << "\r\n";
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
            auto it = db.find(key);
            if (it != db.end()) {
                response << "$" << it->second.size() << "\r\n" << it->second << "\r\n";
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

CommandHandler::~CommandHandler() = default;
