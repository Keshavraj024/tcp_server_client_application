#include "tcp_server.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

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
    std::cout << "Server started, listening for connections " << std::endl;
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == -1)
    {
        perror("Socket creation failed");
    }

    const int reuse = 1;
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
    {
        perror("Failed to set socket option");
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Binding failed");
        return false;
    }

    if (listen(m_serverSocket, 5) == -1)
    {
        perror("Listening failed ");
        return false;
    }

    return true;
}

void TcpServer::stopConnection()
{
    if (m_serverSocket != -1)
    {
        close(m_serverSocket);
        m_serverSocket = -1;
        m_endConnection = false;
    }
}

void TcpServer::listenForConnection()
{
    while (!m_endConnection)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(m_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            perror("Accept Failed ");
            m_endConnection = true;
        }

        handleClient(clientSocket);
    }
}

void TcpServer::handleClient(const int &clientSocket)
{
    char buffer[4096] = {0};

    while (!m_endConnection)
    {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            perror("Client Disconnected ");
            m_endConnection = true;
        }

        sr_test::Output receivedMessage;
        if (receivedMessage.ParseFromArray(buffer, bytesRead))
        {
            std::cout << "Received messagae: ";
            std::cout << "ID : " << receivedMessage.id() << std::endl;
            std::cout << "Timestamp : " << receivedMessage.timestamp().seconds() << " s " << receivedMessage.timestamp().nanos() << " ns" << std::endl;
            std::cout << "Content : " << receivedMessage.content() << std::endl;
        }
        else
        {
            std::cerr << "Error in receiving message " << std::endl;
        }
    }

    stopConnection();
}
