//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_INETADDRESS_H
#define SOCKETFRAMEWORK_INETADDRESS_H

// 对于sockaddr_in的简单封装
//#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
class InetAddress {
public:
    InetAddress() = default;
    explicit InetAddress(uint16_t port,bool loopBackOnly = true, bool ipv6 = false);
    sockaddr * getSocketAddress() const;
    int getStructSize() const;
    std::string toIp() const;
    void setSockAddrInet6(const struct sockaddr_in6& addr6) { addrv6 = addr6; }
    uint16_t toPort() const { return addr.sin_port; }
private:
    union
    {
        struct sockaddr_in6 addrv6;
        struct sockaddr_in addr;
    };
};


#endif //SOCKETFRAMEWORK_INETADDRESS_H
