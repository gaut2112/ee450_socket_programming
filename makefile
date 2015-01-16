CC=g++

LIBS = -lsocket -lnsl -lresolv

all:server doc1 doc2 patient1 patient2

server:
	$(CC) -o server healthcenterserver.cpp $(LIBS)

doc1:
	$(CC) -o doctor1 doctor1.cpp $(LIBS) 

doc2:
	$(CC) -o doctor2 doctor2.cpp $(LIBS)

patient1:
	$(CC) -o patient1 patient1.cpp $(LIBS) 

patient2:
	$(CC) -o patient2 patient2.cpp $(LIBS)

clean:
	rm server
	rm doctor1
	rm doctor2
	rm patient1
	rm patient2


