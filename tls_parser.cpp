#include "tls_parser.hpp"

int hex_string_to_uint8_array(
    const std::string& hex_string, 
    std::vector<uint8_t>& buffer
)
{
    uint16_t cursor = 0;
    while ( cursor < hex_string.size() )
    {
        std::string current_num = hex_string.substr(cursor, 2);
        char *p;
        buffer.push_back( ::strtoul(current_num.c_str(), &p, 16) & 0xff );

        cursor += 2;
    }
    return 0;
}

int load_packet_from_txt(
    const char* file_path,
    std::vector<uint8_t>& buffer
)
{
    std::string file_content;
    std::ifstream raw_string_file(file_path);

    if ( not raw_string_file.is_open() )
    {
        return 1;
    }

    while ( std::getline(raw_string_file, file_content) )
    {
        hex_string_to_uint8_array(file_content, buffer);
        break;
    }
    return 0;
}

bool is_valid_tls_client_hello(
    const uint8_t* packet,
    const std::size_t& packet_size
)
{
    if ( packet[0] != TLSHandshakeConstants::handshake_type )
    {
        return false;
    }

    const uint16_t tls_version = packet[1] << 8 | packet[2];
    if ( 
        tls_version != TLSHandshakeConstants::tls10_type and 
        tls_version != TLSHandshakeConstants::tls12_type
    )
    {
        return false;
    }

    const uint16_t overall_packet_size = packet[3] << 8 | packet[4];
    if ( overall_packet_size != packet_size - 5 )
    {
        return false;
    }

    if ( packet[5] != TLSHandshakeConstants::client_hello_type )
    {
        return false;
    }

    return true;
}

bool get_sni_from_tls_handshake(
    const uint8_t* packet,
    const std::size_t& packet_size,
    std::string& sni_name
)
{
    if ( not is_valid_tls_client_hello(packet, packet_size) )
    {
        return false;
    }

    uint16_t cursor = TLSHandshakeBitIndices::cipher_suite_starts_at;
    uint16_t num_of_cipher_suites = packet[cursor] << 8 | packet[cursor+1];
    cursor += 1;
    cursor += num_of_cipher_suites + 1;

    uint8_t num_of_compression_methods = packet[cursor];
    cursor += num_of_compression_methods + 1;

    uint16_t extension_len = packet[cursor] << 8 | packet[cursor+1];
    cursor += 2;

    while ( cursor < packet_size )
    {
        const uint16_t current_record_type = packet[cursor] << 8   | packet[cursor+1];
        const uint16_t current_record_len  = packet[cursor+2] << 8 | packet[cursor+3];

        cursor += 4;

        if ( current_record_type == TLSHandshakeConstants::sni_record_type )
        {
            return parse_sni_record(packet+cursor, sni_name) == 0;
        }
        else
        {   
            cursor += current_record_len;
        }
    }

    return false;
}

int parse_sni_record(
    const uint8_t* record,
    std::string& output
)
{
    const uint16_t sni_list_len  = record[0] << 8 | record[1];
    const uint8_t  sni_name_type = record[2];

    if ( sni_name_type != TLSHandshakeConstants::sni_hostname_type )
    {
        return 1;
    }

    const uint16_t sni_name_len = record[3] << 8 | record[4];
    output = std::string((char*)(record+5L));

    return 0;
}
