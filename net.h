#ifndef NET_H
#define NET_H

#include <stdint.h>

// 网络错误代码
typedef enum {
    NET_SUCCESS = 0,
    NET_ERR_INIT_FAILED,
    NET_ERR_INVALID_PACKET,
    NET_ERR_CONNECTION,
    NET_ERR_TIMEOUT
} NetError;

// 网络接口结构体
typedef struct {
    uint8_t mac_address[6];
    uint32_t ip_address;
    uint16_t mtu_size;
    uint8_t is_connected;
} NetworkInterface;

// 数据包结构体
typedef struct {
    uint8_t src_mac[6];
    uint8_t dest_mac[6];
    uint16_t protocol;
    uint8_t *payload;
    uint16_t length;
} Packet;

// 基本API声明
NetError net_init(void);
NetError net_send_packet(const Packet *packet);
NetError net_receive_packet(Packet *packet);
NetError net_connect(uint32_t ip);
NetError net_disconnect(void);

// 协议处理接口
void handle_arp_packet(const Packet *packet);
void handle_ip_packet(const Packet *packet);

// 网络工具函数
int net_download(const char *url, FILE *output);

#endif // NET_H