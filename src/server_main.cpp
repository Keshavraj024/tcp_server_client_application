#include "tcp_server.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <memory>
#include "utils.cpp"

int main(int argc, const char *argv[])
{

    const char *defaultFileName = "../config.yaml";
    const char *fileName = (argc > 1) ? argv[1] : defaultFileName;

    auto config = Utils::readConfig(fileName);
    if (not config)
    {
        return 1;
    }
    const int serverPort = config["server"]["port"].as<int>();
    const int interval = config["interval"].as<int>();
    const int maxAttempts = config["max_attempts"].as<int>();

    int reconnectAttempts = 0;

    while (reconnectAttempts < maxAttempts)
    {
        std::unique_ptr<TcpServer> server = std::make_unique<TcpServer>(serverPort);
        if (server->startConnection())
        {
            server->listenForConnection();
        }
        else
        {
            std::cerr << "Server failed to start, reconnecting in " << interval << " seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{interval});
            ++reconnectAttempts;
        }
    }

    std::cout << "Exceeded maximum reconnection attempts. Exiting..." << std::endl;
    return 0;
}