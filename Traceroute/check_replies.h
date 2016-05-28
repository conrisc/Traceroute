#ifndef check_replies_h
#define check_replies_h
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <ctime>


bool check_replies(const int *socket, const int *ttl, int n, const int *id,char (*ip_tab)[3][20],time_t (*time_tab)[3][2]);

#endif
