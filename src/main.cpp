#include "tcp_client.h"
#include "output.pb.h"
#include <chrono>
#include <ctime>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    std::unique_ptr<TcpClient> tcpClient = std::make_unique<TcpClient>("127.0.0.1", 3000);

    if (!tcpClient->clientConnect())
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

        std::this_thread::sleep_for(1s);
    }
}