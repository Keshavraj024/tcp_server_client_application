#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <string>
#include "output.pb.h"
#include <functional>

class TcpServer
{
public:
    using messageCallback = std::function<void(const std::string &)>;
    TcpServer(const size_t &);
    ~TcpServer();

    bool startConnection();
    void stopConnection();

private:
    size_t m_port;
    int m_serverSocket;

    void listenForConnection();
    void handleClient(const int &clientSocket);
};

#endif