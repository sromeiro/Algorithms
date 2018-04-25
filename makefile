CFLAGS=-I${BOOST_HOME} -Wall

all: proj

proj: main.o
	g++ $(CFLAGS) main.o -o proj

main.o: main.cpp
	g++ -Wall -c main.cpp

clean:
	rm -rf *o proj
	rm -rf output.txt
