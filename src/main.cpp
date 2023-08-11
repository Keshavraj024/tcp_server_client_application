#include "tcp_client.h"
#include "tcp_server.h"
#include "output.pb.h"
#include <chrono>
#include <ctime>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

constexpr int PORT = 5000;
constexpr const char *SERVER_ADDRESS = "127.0.0.1";
constexpr int MAX_ATTEMPTS = 5;
constexpr std::chrono::seconds INTERVAL{5s};

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

    std::thread serverThread([]()
                             { TcpServer server(PORT);
                             if(!server.startConnection())
                             {
                                std::cerr << "Server failed to start " << std::endl;

                             } });

    std::unique_ptr<TcpClient>
        tcpClient = std::make_unique<TcpClient>(SERVER_ADDRESS, PORT);

    constexpr int MAX_ATTEMPTS = 5;
    if (!waitForServer(tcpClient, MAX_ATTEMPTS, INTERVAL))
    {
        return 1;
    }
    size_t messageId = 0;
    while (true)
    {
        sr_test::Output out;
        out.set_id(++messageId);

        auto now = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
        google::protobuf::Timestamp *timestampProto = out.mutable_timestamp();
        timestampProto->set_seconds(timestamp);

        int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000;
        timestampProto->set_nanos(timestamp);

        const std::string content = "Hello, Seoul Robotics ";
        out.set_content(content);

        if (!tcpClient->sendMessage(out))
        {
            return 1;
        }

        std::cout << "Message sent " << std::endl;
        std::this_thread::sleep_for(5s);
        std::cout << "Waiting done " << std::endl;
    }
}