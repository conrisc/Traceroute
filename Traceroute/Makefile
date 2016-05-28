traceroute: traceroute.o send_packets.o check_replies.o checksum.o
	g++ -Wall -W traceroute.o send_packets.o check_replies.o checksum.o -o traceroute

traceroute.o: traceroute.cpp
	g++ -Wall -W -c traceroute.cpp -o traceroute.o

send_packets.o: send_packets.cpp
	g++ -Wall -W -c send_packets.cpp -o send_packets.o

check_replies.o: check_replies.cpp
	g++ -Wall -W -c check_replies.cpp -o check_replies.o

checksum.o: checksum.cpp
	g++ -Wall -W -c checksum.cpp -o checksum.o

clean:
	rm -f *.o

distclean:
	rm -f *.o traceroute
