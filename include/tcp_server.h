#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <string>
#include "output.pb.h"
#include <functional>

/**
 * @class TcpServer
 * @brief Represents a TCP server that listens for incoming connections and handles client requests.
 */
class TcpServer
{
public:
    /**
     * @brief Constructor for TcpServer.
     * @param port The port number to listen on.
     */
    TcpServer(const size_t &);
    /**
     * @brief Destructor for TcpServer.
     */
    ~TcpServer();
    /**
     * @brief Starts the server's connection by creating a socket, binding it, and listening for incoming connections.
     * @return True if the server connection is successfully established, false otherwise.
     */
    bool startConnection();
    /**
     * @brief Stops the server's connection by closing the server socket.
     */
    void stopConnection();
    /**
     * @brief Listens for incoming client connections and handles them in a loop.
     */
    void listenForConnection();

private:
    size_t m_port;      ///< The port number to listen on.
    int m_serverSocket; ///< The socket descriptor for the server socket.

    /**
     * @brief Handles communication with a connected client.
     * @param clientSocket The socket descriptor for the connected client.
     */
    void handleClient(const int &clientSocket);
};

#endif