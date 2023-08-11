#include "tcp_server.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std::chrono_literals;

TcpServer::TcpServer(const size_t &port)
    : m_port{port}, m_serverSocket{-1}
{
}

TcpServer::~TcpServer()
{
    stopConnection();
}

bool TcpServer::startConnection()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Binding failed" << std::endl;
        return false;
    }

    if (listen(m_serverSocket, 5) == -1)
    {
        std::cerr << "Listening failed " << std::endl;
        return false;
    }

    listenForConnection();
    return true;
}

void TcpServer::stopConnection()
{
    if (m_serverSocket != -1)
    {
        close(m_serverSocket);
        m_serverSocket = -1;
    }
}

void TcpServer::listenForConnection()
{
    while (true)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(m_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            std::cerr << "Accept Failed " << std::endl;
        }

        handleClient(clientSocket);
        std::this_thread::sleep_for(1s);
    }
}

void TcpServer::handleClient(const int &clientSocket)
{
    char buffer[4096];

    while (true)
    {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            std::cerr << "Client Disconnected" << std::endl;
        }

        sr_test::Output receivedMessage;
        if (receivedMessage.ParseFromArray(buffer, bytesRead))
        {
            std::cout << "Received messagae : ";
            std::cout << "ID : " << receivedMessage.id() << std::endl;
            std::cout << "Timestamp : " << receivedMessage.timestamp().seconds() << receivedMessage.timestamp().nanos() << std::endl;
            std::cout << "Content : " << receivedMessage.content() << std::endl;
        }
        else
        {
            std::cerr << "Received no messaage " << std::endl;
        }
        
    }

    stopConnection();
}