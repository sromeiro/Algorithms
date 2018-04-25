Notes:
	This program accepts a maze text file as input and outputs a solution to "output.txt"
	in the same directory. The input is accepted via command line argument. If no argument
	is given, the program will default to a "input.txt" file. If the given files cannot
	be found in the directory, the program will exit.

Compiling:
	To compile this project simply run the makefile by typing "make" in the command line.

Executing:
	To execute the program, simply type "./proj" in the command line once it's compiled.

Assumptions:
	The program assumes an environmental variable named "BOOST_HOME" that is set up to 
	point to the directory that has the boost library. This program assumes that the 
	needed input file is located in the same directory as the program unless the full
	path is specified.
