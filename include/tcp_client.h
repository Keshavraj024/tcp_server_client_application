#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <memory>

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
    /**
     * Generates a message using the given message ID and content.
     * The message will be formatted as a string containing both the message ID and content.
     *
     * @param messageId The unique identifier for the message.
     * @param content The content of the message to be included.
     * @return A string representing the created message with the message ID, timestamp and content.
     */
    std::string createMessage(size_t messageId, const std::string &content);
    /**
     * Wait for the server connection with retries.
     *
     * This function waits for a successful connection to the server using the provided TcpClient instance,
     * attempting connection with retries. It will make multiple connection attempts with a specified
     * interval between attempts.
     *
     * @param client A unique pointer to the TcpClient instance used for connection.
     * @param maxAttempts The maximum number of connection attempts to be made.
     * @param interval The time interval in seconds between connection attempts.
     * @return True if the connection to the server is successful, false otherwise.
     */
    bool waitForServer(const std::unique_ptr<TcpClient> &client, size_t maxAttempts, int interval);

private:
    int m_clientSocket;          ///< The socket descriptor for the client socket.
    std::string m_serverAddress; ///< The IP address or hostname of the server.
    size_t m_serverPort;         ///< The port number of the server.
};

#endif