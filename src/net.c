
/*
 * Implement the treppe2000 network protocol
 *
 *   Encode the framebuffer using net_encode, then
 *   send it to the treppe2000 daemon via UDP
 *
 *  Commands:
 *
 *       0x23 - Set Channel to 16 bit RGBW value (Direct to serial)
 *       0x42 - Set frame with RGB(W) values
 *
 *
 *       Flags:
 *
 *       Channel 0x00 - 0x10 (stair 0 - 16)
 *
 *       Frame 0x01 RGB
 *             0x02 RGBW
 *             0x04 8 Bit
 *             0x08 16 Bit (Big Endian)
 *
 *
 *       Frame:
 *
 *       16 * RGB(A) Values
 *
 *       [1 Byte Cmd, 1 Byte Frame, Payload]
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net.h"

net_packet* net_packet_alloc(const double* buffer)
{
    size_t width = buffer[0];
    size_t height = buffer[1];

    size_t fragment_size = 8; // RGBW * 2 byes per channels
    size_t buffer_size = width * height * fragment_size + 2;  

    // Allocate buffer
    uint8_t* data = malloc(buffer_size * sizeof(uint8_t));

    // Allocate packet
    net_packet* packet = (net_packet*)malloc(sizeof(net_packet));
    packet->data = data;
    packet->len = buffer_size;

    // Set deterministic fields
    packet->data[0] = NET_CMD_FRAME; // We only support frame data
    packet->data[1] = NET_FRAME_RGBW | NET_ENCODE_16; 

    return packet;
}

void net_packet_free(net_packet* packet)
{
    free((void*)packet->data);
    free(packet);
}

void net_packet_encode(net_packet* packet, const double* buffer)
{
    size_t buffer_len = (size_t)buffer[0] * (size_t)buffer[1] * 4;
    uint16_t* packet_data = (uint16_t*)(packet->data + 2);

    for (size_t i = 0; i < buffer_len; i++) {
        packet_data[i] = htons((uint16_t)(buffer[i+2] * 65535.0));
    }
}

int net_socket_open()
{
     return socket(AF_INET, SOCK_DGRAM, 0);
}

size_t net_packet_send(int sock,
                       const char* hostname,
                       int port,
                       const net_packet* packet)
{
    // We do gethostbyname each time. Maybe the address of the
    // host changed after a while...
    struct hostent *host = gethostbyname(hostname);
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);              
    memcpy(&addr.sin_addr.s_addr,
           host->h_addr,
           host->h_length);

    return sendto(sock,
                  packet->data, packet->len, 0,
                  (const struct sockaddr*)&addr,
                  sizeof(struct sockaddr_in));
}

