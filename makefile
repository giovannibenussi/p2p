# OPCIONES_COMPILADOR=-pg -Wall -g
# OPCIONES_COMPILADOR=-Wall -O2
OPCIONES_COMPILADOR=-Wall -g
CPPSIM=-Isrc $(OPCIONES_COMPILADOR)

all: main.o Peer.o Node.o Transport.o Message.o DNS.o WSE.o LruA.o Lru.o gen_rnd.o Util.o Stats.o Hash.o
	clang++ objects/main.o objects/Peer.o objects/Node.o objects/Transport.o objects/Message.o objects/DNS.o objects/WSE.o objects/LruA.o objects/Lru.o objects/gen_rnd.o objects/Util.o objects/Stats.o objects/Hash.o src/libcppsim.a $(CPPSIM) -L/usr/lib -lssl -lcrypto -o main
	@clear
	@./main 100 5 100

main.o: main.cpp Peer.o
	@clang++ $(CPPSIM) -c main.cpp -o objects/main.o

Peer.o: peer/Peer.cpp
	@clang++ $(CPPSIM) -c peer/Peer.cpp -o objects/Peer.o

Node.o: node/Node.cpp
	@clang++ $(CPPSIM) -c node/Node.cpp -o objects/Node.o

Transport.o: transport/Transport.cpp
	@clang++ $(CPPSIM) -c transport/Transport.cpp -o objects/Transport.o

Message.o: transport/Message.cpp
	@clang++ $(CPPSIM) -c transport/Message.cpp -o objects/Message.o

DNS.o: transport/Dns.cpp
	@clang++ $(CPPSIM) -c transport/Dns.cpp -o objects/DNS.o

WSE.o: wse/WSE.cc
	@clang++ $(CPPSIM) -c wse/WSE.cc -o objects/WSE.o

LruA.o: wse/LruA.cc
	@clang++ $(CPPSIM) -c wse/LruA.cc -o objects/LruA.o

Lru.o: wse/Lru.cc
	@clang++ $(CPPSIM) -c wse/Lru.cc -o objects/Lru.o

gen_rnd.o: generador/gen_rnd.cc
	@clang++ $(CPPSIM) -c generador/gen_rnd.cc -o objects/gen_rnd.o

Util.o: auxiliar/Util.cc
	@clang++ $(CPPSIM) -c auxiliar/Util.cc -o objects/Util.o

Stats.o: Stats.cpp
	@clang++ $(CPPSIM) -c Stats.cpp -o objects/Stats.o

Hash.o: auxiliar/Hash.cc
	@clang++ $(CPPSIM) -c auxiliar/Hash.cc -o objects/Hash.o

main:
	@clang++ main.cpp client/Client.cpp edgeserver/EdgeServer.cpp node/Node.cpp transport/Transport.cpp transport/Message.cpp transport/Dns.cpp wse/WSE.cc wse/LruA.cc generador/gen_rnd.cc auxiliar/Util.cc Stats.cpp Observer.cc src/libcppsim.a -Isrc  -o main
	clear
	@./main



clear:
	rm main ; clear