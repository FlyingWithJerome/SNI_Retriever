#ifndef TLS_PARSER_
#define TLS_PARSER_

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>

namespace TLSHandshakeConstants
{
    constexpr uint8_t handshake_type     = 0x16;
    constexpr uint8_t client_hello_type  = 0x01;
    constexpr uint16_t tls10_type        = 0x0301;
    constexpr uint16_t tls12_type        = 0x0303;

    constexpr uint16_t sni_record_type   = 0x0000;
    constexpr uint8_t  sni_hostname_type = 0x00;
}

namespace TLSHandshakeBitIndices
{
    constexpr uint16_t first_record_starts_at = 5;
    constexpr uint16_t cipher_suite_starts_at = 76;
}

int hex_string_to_uint8_array(
    const std::string&, 
    std::vector<uint8_t>&
);

bool get_sni_from_tls_handshake(
    const uint8_t*,
    const std::size_t&,
    std::string&
);

bool is_valid_tls_client_hello(
    const uint8_t*,
    const std::size_t&
);

int load_packet_from_txt(
    const char*,
    std::vector<uint8_t>&
);

int parse_sni_record(
    const uint8_t*,
    std::string &
);

#endif
