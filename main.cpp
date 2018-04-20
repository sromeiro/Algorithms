#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<cmath>
#include<string>
#include<fstream>
#include<vector>

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

    inFile >> totalRows >> totalColumns;
    cout << "Totalrows: " << totalRows << "\ttotalColumns: " << totalColumns
         << endl;

    //Vector format: vector<DataType> nameOfVector
    //Vector of vectors format: vector < vector<DataType> > nameOfVector
    vector< vector<string> > mazeVector;

    //Loop to store a vector of strings in a vector of vectors
    for(row = 0; row < totalRows; row++)
    {
        //create the vector of strings to store in mazeVector
        vector<string> temp;
        string tempStr;
        for(column = 0; column < totalColumns; column++)
        {
            //Loop that reads the next word/column in this row and stores it
            inFile >> tempStr;
            temp.push_back(tempStr);
        }
        //Store the 8 words/columns inside of mazeVector
        mazeVector.push_back(temp);
    }

    //Loop that reads back out the contents of the maze if needed.
    for(unsigned int i = 0; i < mazeVector.size(); i++)
    {
        for(unsigned int j = 0; j < mazeVector[i].size(); j++)
        {
            cout << mazeVector[i][j] << " ";
        }
        cout << endl;
    }




    //Close the open files
    inFile.close();
    outFile.close();

    //Successful termination of program
    cout << "\nProgram terminated sucesfully" << endl;
}
