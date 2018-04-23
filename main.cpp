#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<cmath>
#include<string>
#include<fstream>
#include<vector>
#include<boost/graph/adjacency_list.hpp>

//Change file name here if needed.
#define FILENAME "apollodiana.txt"

using namespace std;

struct vertex
{
    boost::adjacency_list<>::vertex_descriptor id;
    char color;
    string direction;
};

//===================================TO DO====================================//
/* Need to add edges between vertices depending on the direction given.
*  If direction is E or W just add edges between adjacent vertices +/- 1
*  If direction is N or S add edges between vertices in +/- 8 for top and bottom
*  If direction is NW, SE add edges between vertices in +/- 9 since longer
*  If direction is NE, SW add edges between vertices in +/- 7 since shorter
*/
//============================================================================//

int main()
{
    int totalRows, totalColumns;
    //int row, column;
    boost::adjacency_list<> graph;

    /*
    vertex v1, v2, v3, v4;
    boost::adjacency_list<> graph;

    v1.id = add_vertex(graph);
    v2.id = add_vertex(graph);
    v3.id = add_vertex(graph);
    v4.id = add_vertex(graph);

    typedef boost::adjacency_list<>::vertex_iterator vertIterator;

    pair<vertIterator, vertIterator> vertPair = vertices(graph);

    for (vertIterator it = vertPair.first; it != vertPair.second; ++it)
    {
        std::cout << *it << std::endl;
    }

    pair<boost::adjacency_list<>::edge_descriptor, bool> edge;

    edge = add_edge(v1.id, v2.id, graph);

    typedef boost::adjacency_list<>::edge_iterator edgeIterator;
    pair<edgeIterator, edgeIterator> edgePair;
    edgePair = edges(graph);

    for (edgeIterator eit = edgePair.first; eit != edgePair.second; ++eit)
    {
        cout << *eit << endl;
    }
    */

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

    //Create an array of vertex structures, an array of vertices
    vertex vertexes[totalRows*totalColumns];

    for(int i = 0; i < totalRows*totalColumns; i++)
    {
        //Add vertex to graph
        vertexes[i].id = add_vertex(graph);

        //Get the string to extract color and direction
        string tempStr;
        inFile >> tempStr;

        //Set vertex color
        vertexes[i].color = tempStr[0];
        if(tempStr.length() > 3)
        {
            //Direction is NW, NE, SE, SW
            vertexes[i].direction = tempStr[2];
            vertexes[i].direction += tempStr[3];
        }
        else
        {
            //Direction is N, E, S, W
            vertexes[i].direction = tempStr[2];
        }
        cout << "vertexID: " << vertexes[i].id
             << "\tcolor: "  << vertexes[i].color
             << "\tdirection: " << vertexes[i].direction << endl;
    }

    //Define a new type for simplicity. This will be a vertex iterator type
    typedef boost::adjacency_list<>::vertex_iterator vertIterator;

    //vertices() returns a pair of iterators for begining and end of graph
    pair<vertIterator, vertIterator> vertPair = vertices(graph);

    //Loop through entire graph from first vertex id to end
    for (vertIterator it = vertPair.first; it != vertPair.second; ++it)
    {
        cout << *it << endl;
    }








/* //====================VECTOR IMPLEMENTATION================================//
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
*/ //=========================================================================//



    //Close the open files
    inFile.close();
    outFile.close();

    //Successful termination of program
    cout << "\nProgram terminated sucesfully" << endl;
}
