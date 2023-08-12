#include "tcp_client.h"
#include <arpa/inet.h>
#include <thread>
#include <yaml-cpp/yaml.h>
#include "output.pb.h"



bool waitForServer(const std::unique_ptr<TcpClient> &client, const size_t &maxAttempts, const int &interval)
{
    for (size_t attempt = 0; attempt < maxAttempts; attempt++)
    {
        if (client->clientConnect())
        {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::seconds{interval});
    }
    return false;
}

int main()
{

    YAML::Node config = YAML::LoadFile("../config.yaml");
    if (!config.IsDefined())
    {
        std::cerr << "Error reading config file " << '\n';
        return 1;
    }
    const std::string serverAddress = config["server"]["address"].as<std::string>();
    const int serverPort = config["server"]["port"].as<int>();
    const int interval = config["interval"].as<int>();
    const int maxAttempts = config["max_attempts"].as<int>();
    const int messageIntervalMs = config["message_interval_ms"].as<int>();

    std::unique_ptr<TcpClient>
        tcpClient = std::make_unique<TcpClient>(serverAddress, serverPort);

    size_t messageId = 0;

    while (true)
    {
        sr_test::Output out;
        out.set_id(++messageId);

        const auto now = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
        google::protobuf::Timestamp *timestampProto = out.mutable_timestamp();
        timestampProto->set_seconds(timestamp);
        const int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000;
        timestampProto->set_nanos(nanos);

        const std::string content = "Hello, Seoul Robotics ";
        out.set_content(content);

        std::cout << "Sending Message ID " << messageId << std::endl;

        const std::string messageToSend = out.SerializeAsString();

        while (!tcpClient->sendMessage(messageToSend))
        {
            if (!waitForServer(tcpClient, maxAttempts, interval))
            {
                perror("Server not reachable. Exiting...");
                return 1;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(messageIntervalMs));
    }
}