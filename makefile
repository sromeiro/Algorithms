all: proj

proj: main.o
	g++ -Wall -I$BOOST_HOME main.o -o proj

main.o: main.cpp
	g++ -Wall -c main.cpp

clean:
	rm -rf *o proj
	rm -rf output.txt
