#include "tcp_client.h"
#include <arpa/inet.h>

TcpClient::TcpClient(const std::string &serverAddress, const size_t &serverPort)
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

bool TcpClient::clientConnect()
{
    m_clientSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (m_clientSocket < 0)
    {
        std::cerr << "Socket Connection Failed " << std::endl;
        return false;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);
    if (inet_pton(AF_INET, m_serverAddress.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid Address or address not supported " << std::endl;
        close(m_clientSocket);
        return false;
    }

    auto status = connect(m_clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (status < 0)
    {
        std::cerr << "Connection Failed " << std::endl;
        close(m_clientSocket);
        return false;
    }

    return true;
}

bool TcpClient::sendMessage(const google::protobuf::Message &message)
{
    std::string messageToSend = message.SerializeAsString();
    size_t messageSize = messageToSend.size();

    if (send(m_clientSocket, &messageSize, sizeof(int), 0 == -1))
    {
        std::cerr << "Failed to send message size " << std::endl;
        return false;
    }

    if (send(m_clientSocket, messageToSend.c_str(), messageSize, 0) == -1)
    {
        std::cerr << "Failed to send message of size " << std::endl;
        return false;
    }

    return true;
}