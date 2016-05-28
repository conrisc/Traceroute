//Konrad Cielecki

#include <iostream>
#include <ctime>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include "send_packets.h"
#include "check_replies.h"

using namespace std;

// Funkcja sprawdzajaca poprawnosc IP
bool is_ip(char tab[]) {
  char *ptr;
  long int li;
  int dot = 0;
  //Sprawdzanie znakow i liczenie kropek
  for (ptr = tab;*ptr!='\0';ptr++) {
    if (*ptr=='.' && *(ptr-1)=='.') return false;
    else if (*ptr=='.') dot++;
    else if ( (int)*ptr<48 || 57<(int)*ptr ) return false;
  }
  //Sprawdzanie zakresu
  for (ptr=tab;*ptr!='\0';) {
    li = strtol (ptr,&ptr,10);
    if (li<0 || 255<li) return false;
    if (*ptr!='\0')
      ptr++;
  }
  if (dot!=3 ||  tab[0]=='.' || *(ptr-1)=='.') return false;
  return true;
}

int main(int argc,char *argv[]) {

  //Sprawdzanie obecnosci oraz poprawnosci argumentu (ip)
  if (argc < 2 || is_ip(argv[1])==false) {
    cerr<<"Invalid argument"<<endl;
    return 1;
  }

  // Tworzenie gniazda
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   if (sockfd == -1) {
    perror(NULL);
     return 1;
  }
  
  // Ustawianie ID
  int id = getpid();

  bool dst_reached = false;


  for (int ttl= 1;ttl<=30 && dst_reached==false;ttl++) {
    // Ustawianie TTL
    setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    //Tablica traceroute
    char ip_tab[3][20];
    //Tablica czasu
    time_t time_tab[3][2]; //
    
    // Wysylanie 3 pakietow
    send_packets(&sockfd,&ttl,3,argv[1],&id,&time_tab);
    //Poczekaj sekunde
    sleep(1);
    //Sprawdz 3 odpowiedzi
    dst_reached = check_replies(&sockfd,&ttl,3,&id,&ip_tab,&time_tab);
    if (errno !=0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      perror(NULL);
      return 1;
    }

    //Wypisywanie danych
    cout<<ttl<<". ";
    if (ip_tab[0][0]=='\0')
      cout<<"*";
    else {
      cout<<ip_tab[0]<<" ";
      if (strcmp(ip_tab[1],ip_tab[0])!=0)
	cout<<ip_tab[1]<<" ";
      if (strcmp(ip_tab[2],ip_tab[0])!=0 && strcmp(ip_tab[2],ip_tab[1])!=0)
	cout<<ip_tab[2]<<" ";
      
      
      if (time_tab[0][1]!=0 && time_tab[1][1]!=0 && time_tab[2][1]!=0) {
	int ave_t =(time_tab[0][1]+time_tab[1][1]+time_tab[2][1])/3;
	cout<<" "<<ave_t<<"ms";
      }
      else
	cout<<"???"; 
    }
    cout<<endl;    
  }
  
  return 0;

}



