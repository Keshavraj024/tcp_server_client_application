#include "tcp_client.h"
#include <arpa/inet.h>
#include <thread>
#include <yaml-cpp/yaml.h>
#include "utils.cpp"
#include "output.pb.h"

int main(int argc, const char *argv[])
{
    const char *defaultFileName = "../config.yaml";
    const char *fileName = (argc > 1) ? argv[1] : defaultFileName;

    auto config = Utils::readConfig(fileName);
    if (not config)
    {
        return 1;
    }
    const std::string serverAddress = config["server"]["address"].as<std::string>();
    const int serverPort = config["server"]["port"].as<int>();
    const int interval = config["interval"].as<int>();
    const int maxAttempts = config["max_attempts"].as<int>();
    const int messageIntervalMs = config["message_interval_ms"].as<int>();
    const std::string content = config["message_to_send"].as<std::string>();

    std::unique_ptr<TcpClient>
        tcpClient = std::make_unique<TcpClient>(serverAddress, serverPort);

    size_t messageId = 0;
    int endIndex = 0;

    while (true)
    {

        if (endIndex == content.length())
            endIndex = 0;
        const std::string messageToSend = tcpClient->createMessage(messageId++, content.substr(0, endIndex));

        while (!tcpClient->sendMessage(messageToSend))
        {
            if (!tcpClient->waitForServer(tcpClient, maxAttempts, interval))
            {
                perror("Server not reachable. Exiting...");
                return 1;
            }
        }
        endIndex++;
        std::this_thread::sleep_for(std::chrono::milliseconds(messageIntervalMs));
    }
}