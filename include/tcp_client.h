#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <iostream>
#include <string>
#include <ctime>
#include <sys/socket.h>
#include <google/protobuf/message.h>

class TcpClient
{
public:
    TcpClient(const std::string &serverAddress, const size_t &serverPort);
    ~TcpClient();
    bool clientConnect();
    bool sendMessage(const std::string &message);

private:
    int m_clientSocket;
    std::string m_serverAddress;
    size_t m_serverPort;
};

#endif