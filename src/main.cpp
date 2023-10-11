#include "WsServer.h"

#include <iostream>
#include <string>

const std::string kBindAddress = "127.0.0.1";
constexpr uint16_t kPort = 18080;

int main(int argc, char* argv[]) {
    WsServer server(kBindAddress, kPort);

    std::cout << "Server started" << std::endl;
    while (true) {
        std::string command;
        std::cin >> command;
        if (command == "q" || command == "quit") {
            std::cout << "Exiting..." << std::endl;
            break;
        } else {
            if (std::cin.fail() || std::cin.eof()) {
                std::cout << "Input error, exiting..." << std::endl;
                break;
            } else {
                std::cout << "Invalid command. Enter \"q\" to quit" << std::endl;
            }
        }
    }
    return 0;
}
