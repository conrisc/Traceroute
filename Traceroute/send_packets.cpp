//Konrad Cielecki
#include "send_packets.h"

void send_packets(const int *socket, const int *ttl, int n, const char *ip, const int *id,time_t (*time_tab)[3][2]) {

  // Adresowanie
  sockaddr_in recipient;
  bzero (&recipient, sizeof(recipient));
  recipient.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &recipient.sin_addr);
  
  // Tworzenie pakietu ICMP
  icmphdr icmp_header;
  icmp_header.type = ICMP_ECHO;
  icmp_header.code = 0;
 
  // Wysylanie n pakietow
  for(int i=1;i<=n;i++) {

    // Ustawianie pakietu
    icmp_header.un.echo.id = *id;
    icmp_header.un.echo.sequence = 3*(*ttl-1)+i;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum(&icmp_header, sizeof(icmp_header));

    // Wysylanie pakietu
    sendto (
	    *socket,
	    &icmp_header,
	    sizeof(icmp_header),
	    0,
	    (sockaddr*)&recipient,
	    sizeof(recipient)
    );
    (*time_tab)[i-1][0] = clock();
    (*time_tab)[i-1][1] = 0;
  }
}
