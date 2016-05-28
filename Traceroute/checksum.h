#ifndef checksum_h
#define checksum_h
#include <arpa/inet.h>
#include <assert.h>

u_int16_t compute_icmp_checksum(const void *buff, int length);

#endif

