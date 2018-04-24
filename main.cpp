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
/* Need to figure out how to use BFS in Boost library to traverse my graph
*  and return the shortest path between two points.
*/
//============================================================================//

int main()
{
    int totalRows, totalColumns, totalVertices;
    //int row, column;

    //Define Edge Weight Property type
    typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
    //Define adjacency list directed graph type
    typedef boost:: adjacency_list< boost::listS,
                                    boost::vecS,
                                    boost::directedS,
                                    boost::no_property,
                                    EdgeWeightProperty > Graph;

    //Define edge edge_descriptor NOT NEEDED?
    //typedef Graph::edge_descriptor Edge;

    //Create graph
    Graph graph;

    pair<boost::adjacency_list<>::edge_descriptor, bool> edge;

    //Create Edge Weight Map to store edge weights
    boost::property_map < Graph, boost::edge_weight_t >::type EdgeWeightMap;
    EdgeWeightMap = get(boost::edge_weight, graph);

    //Edge iterators to iterate through all edges of graph
    boost::graph_traits< Graph >::edge_iterator edge_it, edge_end;

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
    totalVertices = totalRows * totalColumns;
    cout << "Totalrows: " << totalRows << "\ttotalColumns: " << totalColumns
         << endl;

    //Create an array of vertex structures, an array of vertices
    vertex vertexes[totalVertices];

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
            if(i == 63)
                continue;
            //Direction is N, E, S, W
            vertexes[i].direction = tempStr[2];
        }
        // cout << "vertexID: " << vertexes[i].id
        //      << "\tcolor: "  << vertexes[i].color
        //      << "\tdirection: " << vertexes[i].direction << endl;
    }

    // cout << "Target ID: " << vertexes[63].id
    //      << " Target color: " << vertexes[63].color
    //      << " Target direction: " << vertexes[63].direction << endl;




    //For loop to add correct edges between vertices
    for(int i = 0; i < totalRows * totalColumns; i++)
    {
        //Variable j to keep track of vertex we're connecting an edge to
        int j;
        if(vertexes[i].direction.compare("E") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j < (totalRows + i) - (i % totalRows); j++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j < (totalRows + i) - (i % totalRows); j++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("W") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    //cout << "i = " << i << endl;
                    // cout << "Math: \n" << "i / totalColumns = " << i/totalColumns
                    //     << "\ni / totalColumns * totalColumns = " << (i/totalColumns) * totalColumns
                    //     << endl;
                    for(j = i; j >= (i/totalColumns) * totalColumns; j--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    //cout << "i = " << i << endl;
                    // cout << "Math: \n" << "i / totalColumns = " << i/totalColumns
                    //     << "\ni / totalColumns * totalColumns = " << (i/totalColumns) * totalColumns
                    //     << endl;
                    for(j = i; j >= (i/totalColumns) * totalColumns; j--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("N") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j >= 0; j -= 8)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j >= 0; j -= 8)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("S") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j <= 63; j += 8)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j <= 63; j += 8)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
            }
        }
        else if (vertexes[i].direction.compare("NW") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= 9, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= 9, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("SE") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j <= 63 && x < (totalRows + i) - (i % totalRows) ; j += 9, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j <= 63 && x < (totalRows + i) - (i % totalRows) ; j += 9, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "FOUND TARGET " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("NE") == 0)
        {
            // cout << "\nFound: " << vertexes[i].direction << endl;
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x < (totalRows + i) - (i % totalRows) ; j -= 7, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x < (totalRows + i) - (i % totalRows) ; j -= 7, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("SW") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            // cout << "\nFound: " << vertexes[i].direction << endl;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j <= 63 && x >= (i/totalColumns) * totalColumns; j += 7, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER find this
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j <= 63 && x >= (i/totalColumns) * totalColumns; j += 7, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexes[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER found
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
            }
        }
    }






    //Prints all vertices in the map and weight of edges connected b/w them
    cout << "Source" << "\tTarget" << "\tWeight" << endl;
    for(tie(edge_it, edge_end) = boost::edges(graph); edge_it != edge_end; ++edge_it)
    {
        cout << boost::source(*edge_it, graph) << "\t"
             << boost::target(*edge_it, graph) << "\t"
             << EdgeWeightMap[*edge_it] << endl;
    }


/*
    //Define a new type for simplicity. This will be a vertex iterator type
    typedef boost::adjacency_list<>::vertex_iterator vertIterator;

    //vertices() returns a pair of iterators for begining and end of graph
    pair<vertIterator, vertIterator> vertPair = vertices(graph);

    //Loop through entire graph from first vertex id to end
    for (vertIterator it = vertPair.first; it != vertPair.second; ++it)
    {
        //cout << *it << endl;
    }
*/







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
