#include "tcp_server.h"
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

constexpr int PORT = 8080;
constexpr int MAX_ATTEMPTS = 5;
constexpr std::chrono::seconds INTERVAL{5s};

int main()
{

    int reconnectAttempts = 0;

    while (reconnectAttempts < MAX_ATTEMPTS)
    {
        TcpServer server(PORT);
        if (server.startConnection())
        {
            server.listenForConnection();
        }
        else
        {
            std::cerr << "Server failed to start, reconnecting in " << INTERVAL.count() << " seconds" << std::endl;
            std::this_thread::sleep_for(INTERVAL);
            ++reconnectAttempts;
        }
    }

    std::cout << "Exceeded maximum reconnection attempts. Exiting..." << std::endl;
    return 0;
}