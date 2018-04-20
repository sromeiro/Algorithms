#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<cmath>
#include<string>
#include<fstream>

#define FILENAME "apollodiana.txt" //Change file name here if needed.

using namespace std;

int main()
{
    int totalRows, totalColumns, row, column;

    //Create the input file object
    ifstream inFile;
    inFile.open(FILENAME);
    if (!inFile)
    {
        // call system to stops
        cerr << "Unable to open file";
        exit(1);
    }

    //Create the output file object
    ofstream outFile;
    outFile.open("output.txt");
    if (!outFile)
    {
        // call system to stop
        cerr << "Unable to create file";
        exit(1);
    }



    //Successful termination of program
    cout << "Program terminated sucesfully" << endl;
}
