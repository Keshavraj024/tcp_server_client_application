#include "tcp_server.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <memory>

int main()
{
    YAML::Node config = YAML::LoadFile("../config.yaml");
    if (!config.IsDefined())
    {
        std::cerr << "Error reading config file " << '\n';
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