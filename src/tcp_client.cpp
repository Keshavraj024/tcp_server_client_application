
#include <ctime>
#include <sys/socket.h>
#include <google/protobuf/message.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include "tcp_client.h"
#include "output.pb.h"

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

bool TcpClient::sendMessage(const std::string &messageToSend)
{
    size_t messageSize = messageToSend.size();

    if (send(m_clientSocket, messageToSend.c_str(), messageSize, 0) == -1)
    {
        close(m_clientSocket);
        return false;
    }
    return true;
}
