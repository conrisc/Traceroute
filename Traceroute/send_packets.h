#ifndef send_packets_h
#define send_packets_h
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include "checksum.h"


void send_packets(const int *socket, const int *ttl, int n, const char *ip, const int *id,time_t (*time_tab)[3][2]);

#endif

