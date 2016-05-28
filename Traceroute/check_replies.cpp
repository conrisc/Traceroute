//Konrad Cielecki 
#include "check_replies.h"

using namespace std;

bool check_replies(const int *socket, const int *ttl, int n, const int *id,char (*ip_tab)[3][20],time_t (*time_tab)[3][2]) {
  sockaddr_in sender;
  socklen_t sender_len = sizeof(sender);
  u_int8_t buffer[IP_MAXPACKET+1];
  int p_type,p_code,p_id,p_seq;
  bool reach_dst = false;
  
  for (int i=0;i<n;) {

    // Pobieranie pakietu
    ssize_t packet_len = recvfrom (*socket,
			   buffer,
			   IP_MAXPACKET,
			   MSG_DONTWAIT,
			   (sockaddr*)&sender, &sender_len);

    if (packet_len>0) {   //Jezeli otrzymano pakiet
      (*time_tab)[i][1] = clock()-(*time_tab)[i][0];
      // Pobieranie adresu IP z ktorego przyszla odpowiedz
      inet_ntop(AF_INET,&(sender.sin_addr),(*ip_tab)[i],sizeof((*ip_tab)[i]));
      //Podzial pakietu
      iphdr* ip_header = (iphdr*) buffer;
      u_int8_t* icmp_packet = buffer + 4 * ip_header->ihl;
      icmphdr* icmp_header = (icmphdr*) icmp_packet;

      // Wyciaganie Type,Code, ID i SEQ
      p_type = int(icmp_header->type);
      p_code = int(icmp_header->code);
      //p_code = int(icmp_header->code);
      if (p_type == ICMP_ECHOREPLY ) {          //echo reply
	p_id= int(icmp_header->un.echo.id);
	p_seq = int(icmp_header->un.echo.sequence);
      }
      else if (p_type == ICMP_TIME_EXCEEDED && p_code == ICMP_EXC_TTL) {    //TTL exceeded
	icmphdr* icmp_header_old = (icmphdr*) (icmp_packet+28);
	p_id = int(icmp_header_old->un.echo.id);
	p_seq = int(icmp_header_old->un.echo.sequence);
      }
      else
	p_id = -1;

      //Jezeli otrzymano dobry pakiet
      if ( *id == p_id && *ttl == (p_seq-1)/3+1 ) {
	i++;
	if (p_type==0) reach_dst = true;
      }
      else
	(*time_tab)[i][1] = 0;
      
    }
    else {  //Jezeli nie otrzymano zadnego pakietu
      (*ip_tab)[0][0] = '\0';
      return false;
    }

  }
  if (reach_dst==true) return true;
  return false;
}
