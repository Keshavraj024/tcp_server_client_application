#include "tcp_client.h"
#include <arpa/inet.h>
#include <thread>
#include "output.pb.h"

using namespace std::chrono_literals;

constexpr int PORT = 8080;
constexpr const char *SERVER_ADDRESS = "127.0.0.1";
constexpr int MAX_ATTEMPTS = 5;

bool waitForServer(const std::unique_ptr<TcpClient> &client, const size_t &maxAttempts, const std::chrono::seconds &interval)
{
    for (size_t attempt = 0; attempt < maxAttempts; attempt++)
    {
        if (client->clientConnect())
        {
            return true;
        }
        std::this_thread::sleep_for(interval);
    }
    return false;
}

int main()
{

    std::unique_ptr<TcpClient>
        tcpClient = std::make_unique<TcpClient>(SERVER_ADDRESS, PORT);

    auto INTERVAL = std::chrono::seconds{5};
    size_t messageId = 0;
    while (true)
    {
        sr_test::Output out;
        out.set_id(++messageId);

        std::cout << "Message Id " << messageId << std::endl;

        auto now = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
        google::protobuf::Timestamp *timestampProto = out.mutable_timestamp();
        timestampProto->set_seconds(timestamp);

        int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000;
        timestampProto->set_nanos(timestamp);

        const std::string content = "Hello, Seoul Robotics ";
        out.set_content(content);

        const std::string messageToSend = out.SerializeAsString();

        while (!tcpClient->sendMessage(messageToSend))
        {
            std::cerr << "Failed to send message. Reconnecting..." << std::endl;
            if (!waitForServer(tcpClient, MAX_ATTEMPTS, INTERVAL))
            {
                std::cerr << "Server not reachable. Exiting..." << std::endl;
                return 1;
            }
        }
        std::cout << "Message sent " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}