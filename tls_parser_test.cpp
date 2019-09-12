#include <gtest/gtest.h>
#include "tls_parser.hpp"

TEST(TLSParserTest, TestTXTLoader)
{
    std::vector<uint8_t> buf;
    int status = load_packet_from_txt(
        "sample_tls_handshake_github.txt",
        buf
    );

    ASSERT_EQ(buf[0], 0x16);
    ASSERT_EQ(buf[1], 0x03);
    ASSERT_EQ(buf[2], 0x01);
}

TEST(TLSParserTest, TestTLSClientHelloIdentifier)
{
    std::vector<uint8_t> buf;
    int status = load_packet_from_txt(
        "sample_tls_handshake_github.txt",
        buf
    );
    ASSERT_TRUE( 
        is_valid_tls_client_hello(
            buf.data(),
            buf.size()
        )
    );
}

TEST(TLSParserTest, TestTLSClientHelloParser)
{
    std::vector<uint8_t> buf;
    int status = load_packet_from_txt(
        "sample_tls_handshake_live_github.txt",
        buf
    );
    std::string placeholder;
    ASSERT_TRUE( 
        get_sni_from_tls_handshake(
            buf.data(),
            buf.size(),
            placeholder
        )
    );
    ASSERT_EQ(placeholder, "live.github.com");
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
