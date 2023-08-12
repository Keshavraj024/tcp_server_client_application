#include <gtest/gtest.h>
#include "tcp_client.h"

TEST(TcpClientTest, StartClientConnection)
{
    TcpClient client("127.0.0.1", 8080);
    ASSERT_FALSE(client.clientConnect());
}

TEST(TcpClientTest, SendMessage)
{
    TcpClient client("127.0.0.1", 8080);
    ASSERT_FALSE(client.sendMessage("null"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}