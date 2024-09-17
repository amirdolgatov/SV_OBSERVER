//
// Created by amir on 13.09.24.
//

#ifndef IEC61850_OBSERVER_BPF_SOCKET_H
#define IEC61850_OBSERVER_BPF_SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/filter.h>
#include <stdio.h>
#include <cstdlib>
#include <signal.h>
#include <cstring>
#include <netinet/ether.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

// attach the filter to the socket
// the filter code is generated by running: tcpdump tcp
struct sock_filter BPF_code[] = {
        { 0x28, 0, 0, 0x000000010 },
        { 0x15, 0, 1, 0x0000088ba },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 }
};


class BPF_Socket{

    BPF_Socket(const std::string& iface):opt{iface.c_str()}
    {
        Filter.len = sizeof(BPF_code)/sizeof(BPF_code[0]);
        Filter.filter = BPF_code;
    }

    void create(){
        if ((sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
            perror("socket");
            exit(1);
        }

        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, opt, strlen(opt) + 1) < 0) {
            perror("setsockopt bind device");
            close(sock);
            exit(1);
        }

        struct packet_mreq mr;

        mr.mr_type = PACKET_MR_PROMISC;

        if (setsockopt(sock, SOL_SOCKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
            perror("setsockopt attach filter");
            close(sock);
            exit(1);
        }

        if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &Filter, sizeof(Filter)) < 0) {
            perror("setsockopt attach filter");
            close(sock);
            exit(1);
        }
    }

    void read(){
        char buffer[2048];

        while(1) {
            usleep(100);
            printf("-----------\n");
            int n = recvfrom(sock, buffer, 2048, 0, NULL, NULL);
            printf("%d bytes read\n", n);
            printf("%2x%2x \n", buffer[12], buffer[13]);

        }
    }

    // error prone code, .len field should be consistent with the real length of the filter code array
    struct sock_fprog Filter;
    int sock;
    // bind to eth0 interface only
    const char *opt;
};


#endif //IEC61850_OBSERVER_BPF_SOCKET_H