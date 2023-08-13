#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>

/**
 * @class TcpClient
 * @brief Represents a TCP client that connects to a server and sends messages.
 */
class TcpClient
{
public:
    /**
     * @brief Constructor for TcpClient.
     * @param serverAddress The IP address or hostname of the server to connect to.
     * @param serverPort The port number of the server to connect to.
     */
    TcpClient(const std::string &serverAddress, size_t serverPort);
    /**
     * @brief Destructor for TcpClient.
     */
    ~TcpClient();
    /**
     * @brief Establishes a connection to the server.
     * @return True if the connection is successful, false otherwise.
     */
    bool clientConnect();
    /**
     * @brief Sends a message to the server.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool sendMessage(const std::string &message);
    std::string createMessage(size_t messageId, const std::string &content);

private:
    int m_clientSocket;          ///< The socket descriptor for the client socket.
    std::string m_serverAddress; ///< The IP address or hostname of the server.
    size_t m_serverPort;         ///< The port number of the server.
};

#endif