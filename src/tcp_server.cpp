#include "tcp_server.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

TcpServer::TcpServer(size_t port)
    : m_port{port}, m_serverSocket{-1}
{
}

TcpServer::~TcpServer()
{
    stopConnection();
}

bool TcpServer::startConnection()
{
    std::cout << "Server started, listening for incoming messages " << std::endl;
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed
    if (m_serverSocket == -1)
    {
        perror("Socket creation failed");
        return false;
    }

    const int reuse = 1;

    // Set socket option to reuse address
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
    {
        perror("Failed to set socket option");
        stopConnection();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the specified address and port
    if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Binding failed");
        stopConnection();
        return false;
    }

    // Listen for incoming connections
    if (listen(m_serverSocket, 5) == -1)
    {
        perror("Listening failed ");
        stopConnection();
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

void TcpServer::handleClient(int clientSocket)
{

    while (!m_endConnection)
    {
        size_t messageSize;
        
        recv(clientSocket, &messageSize, sizeof(messageSize), 0);

        std::cout << messageSize << std::endl;

        char buffer[messageSize + 1];
        int bytesRead = recv(clientSocket, buffer, messageSize, 0);

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
            std::cout << "Timestamp : " << receivedMessage.timestamp().seconds() << "s " << receivedMessage.timestamp().nanos() << "ms" << std::endl;
            std::cout << "Content : " << receivedMessage.content() << std::endl;
        }
        else
        {
            std::cerr << "Error in receiving message " << std::endl;
        }
    }

    stopConnection();
}
