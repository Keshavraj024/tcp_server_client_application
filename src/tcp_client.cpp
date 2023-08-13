
#include <ctime>
#include <sys/socket.h>
#include <google/protobuf/message.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include "tcp_client.h"
#include "output.pb.h"

TcpClient::TcpClient(const std::string &serverAddress, size_t serverPort)
    : m_clientSocket{-1}, m_serverAddress{serverAddress}, m_serverPort{serverPort}
{
}

TcpClient::~TcpClient()
{
    if (m_clientSocket != -1)
    {
        close(m_clientSocket);
    }
}

bool TcpClient::waitForServer(const std::unique_ptr<TcpClient> &client, size_t maxAttempts, int interval)
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

bool TcpClient::clientConnect()
{
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientSocket < 0)
    {
        perror("Socket Connection Failed");
        return false;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);

    if (inet_pton(AF_INET, m_serverAddress.c_str(), &serverAddr.sin_addr) <= 0)
    {
        perror("Invalid Address or address not supported");
        close(m_clientSocket);
        return false;
    }

    if (connect(m_clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Connection Failed");
        close(m_clientSocket);
        return false;
    }

    return true;
}

std::string TcpClient::createMessage(size_t messageId, const std::string &content)
{
    sr_test::Output out;
    out.set_id(++messageId);

    const auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    google::protobuf::Timestamp *timestampProto = out.mutable_timestamp();
    timestampProto->set_seconds(timestamp);
    const int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000;
    timestampProto->set_nanos(nanos);

    out.set_content(content);

    std::cout << "Sending Message ID " << messageId << std::endl;

    return out.SerializeAsString();
}

bool TcpClient::sendMessage(const std::string &messageToSend)
{
    size_t messageSize = messageToSend.size();

    if (send(m_clientSocket, &messageSize, sizeof(messageSize), 0) == -1 || send(m_clientSocket, messageToSend.c_str(), messageSize, 0) == -1)
    {
        close(m_clientSocket);
        return false;
    }
    return true;
}
