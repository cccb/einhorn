#ifndef NET_H
#define NET_H

typedef enum {
    NET_CMD_CHANNEL = 0x23,
    NET_CMD_FRAME = 0x42,
}

typedef enum {
    NET_FRAME_RGB  = 0x01,
    NET_FRAME_RGBW = 0x02,
    NET_ENCODE_8   = 0x04,
    NET_ENCODE_16  = 0x08, // 16 Bit, (Big Endian)
} net_frame_encoding;

typedef struct {
    const char* data;
    size_t len;
} net_packet;


net_packet* net_packet_alloc(const double* buffer, net_frame_encoding flags);
void net_packet_free(net_packet* packet);
void net_packet_encode(net_packet* packet, const double* buffer);
int net_send(const char* host, int port, net_packet* packet);

#endif
