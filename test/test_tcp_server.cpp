#include <gtest/gtest.h>
#include "tcp_server.h"

TEST(TcpServerTest, StartConnectionSuccess)
{
    TcpServer server(8080);
    ASSERT_TRUE(server.startConnection());
}

TEST(TcpServerTest, StartConnectionFailure)
{
    TcpServer server(808);
    ASSERT_FALSE(server.startConnection());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
