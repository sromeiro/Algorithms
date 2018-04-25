#include<iostream>
#include<stdlib.h>
#include<stdexcept>
#include<string>
#include<fstream>
#include<vector>
#include<stack>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/breadth_first_search.hpp>
//For Version 1
#include<boost/pending/indirect_cmp.hpp> //NEEDED?
#include<boost/range/irange.hpp> //NEEDED?
//For Version 2
#include <algorithm>
#include <utility>
#include <boost/config.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp>



//Change file name here if needed.
#define FILENAME "apollodiana.txt"

//==============================FOR VERSION 1=================================//
/*
template < typename TimeMap > class bfs_time_visitor:public boost::default_bfs_visitor
{
    typedef typename boost::property_traits < TimeMap >::value_type T;
public:
    bfs_time_visitor(TimeMap tmap, T & t):m_timemap(tmap), m_time(t) { }
    template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
    {
        put(m_timemap, u, m_time++);
    }
    TimeMap m_timemap;
    T & m_time;
};
*/
//============================================================================//

//==============================FOR VERSION 2=================================//

template <class ParentDecorator>
struct print_parent {
  print_parent(const ParentDecorator& p_) : p(p_) { }
  template <class Vertex>
  void operator()(const Vertex& v) const {
    std::cout << "parent[" << v << "] = " <<  p[v]  << std::endl;
  }
  ParentDecorator p;
};

template <class NewGraph, class Tag>
struct graph_copier
  : public boost::base_visitor<graph_copier<NewGraph, Tag> >
{
  typedef Tag event_filter;

  graph_copier(NewGraph& graph) : new_g(graph) { }

  template <class Edge, class Graph>
  void operator()(Edge e, Graph& g) {
    boost::add_edge(boost::source(e, g), boost::target(e, g), new_g);
  }
private:
  NewGraph& new_g;
};

template <class NewGraph, class Tag>
inline graph_copier<NewGraph, Tag>
copy_graph(NewGraph& g, Tag) {
  return graph_copier<NewGraph, Tag>(g);
}

//============================================================================//

using namespace std;
struct vert
{
    boost::adjacency_list<>::vertex_descriptor id;
    char color;
    string direction;

    //Vector of maps that stores the target vertex and the edge weight to it
    vector <  pair<boost::adjacency_list<>::vertex_descriptor, int> > target;
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
    vert v1, v2, v3, v4;
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
    vert vertexes[totalVertices];

    for(int i = 0; i < totalVertices; i++)
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
            if(i == totalVertices - 1)
            {
                cout << "Skipping i = " << i << endl;
                cout << "vertexID: " << vertexes[i].id
                     << "\tcolor: "  << vertexes[i].color
                     << "\tdirection: " << vertexes[i].direction << endl;
                continue;
            }

            //Direction is N, E, S, W
            vertexes[i].direction = tempStr[2];
        }
        cout << "vertexID: " << vertexes[i].id
             << "\tcolor: "  << vertexes[i].color
             << "\tdirection: " << vertexes[i].direction << endl;
    }

    // cout << "Target ID: " << vertexes[63].id
    //      << " Target color: " << vertexes[63].color
    //      << " Target direction: " << vertexes[63].direction << endl;




    //For loop to add correct edges between vertices
    for(int i = 0; i < totalVertices; i++)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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
                    for(j = i; j >= 0; j -= totalRows)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i; j >= 0; j -= totalRows)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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
                    for(j = i; j <= totalVertices - 1; j += totalRows)
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
                    for(j = i; j <= totalVertices - 1; j += totalRows)
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
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= totalRows + 1, x--)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= totalRows + 1, x--)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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
                    for(j = i, x = i; j <= totalVertices - 1 && x < (totalRows + i) - (i % totalRows) ; j += totalRows + 1, x++)
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
                    for(j = i, x = i; j <= totalVertices && x < (totalRows + i) - (i % totalRows) ; j += totalRows + 1, x++)
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
                    for(j = i, x = i; j >= 0 && x < (totalRows + i) - (i % totalRows) ; j -= totalRows - 1, x++)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j >= 0 && x < (totalRows + i) - (i % totalRows) ; j -= totalRows - 1, x++)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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
                    for(j = i, x = i; j <= totalVertices - 1 && x >= (i/totalColumns) * totalColumns; j += totalRows - 1, x--)
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
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            cout << "FOUND TARGET " << vertexes[j].color << " at vertex " << vertexes[j].id << endl;
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    // cout << "I'm a " << vertexes[i].color << " vertex at " << vertexes[i].id << endl;
                    for(j = i, x = i; j <= totalVertices - 1 && x >= (i/totalColumns) * totalColumns; j += totalRows - 1, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                            // cout << "Found a " << vertexes[j].color
                            // << " at vertex " << vertexid <es[j].id
                            // << " with edge weight: " << edgeWeight << endl;
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Probably NEVER found
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
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



//==========================GET BFS WORKING V1================================//
/*
    typedef boost::graph_traits < Graph >::vertices_size_type Size;
    vector < Size > dtime(num_vertices(graph));
    typedef boost::iterator_property_map<vector<Size>::iterator,
                                         boost::property_map<Graph, boost::vertex_index_t>::const_type> dtime_pm_type;

    dtime_pm_type dtime_pm(dtime.begin(), get(boost::vertex_index, graph));

    Size time = 0;
    bfs_time_visitor < dtime_pm_type >vis(dtime_pm, time);

    //call BFS
    breadth_first_search(graph, vertexes[0].id, visitor(vis));

    //Now sort the order of vertices by their discovery time
    vector<boost::graph_traits<Graph>::vertices_size_type> discover_order(vertexes[63].id);
    boost::integer_range < int >range(0, 64);
    copy(range.begin(), range.end(), discover_order.begin());

    sort(discover_order.begin(), discover_order.end(), boost::indirect_cmp
        < dtime_pm_type, less <Size > >(dtime_pm));

    for(unsigned int i = 0; i <= 63; i++)
    {
        cout << "i = " << i << "\tdtime_pm = " << dtime_pm[i] << endl;
    }


    //Print the order of discovery
    //cout << "Size of vertexes: " << sizeof(vertexes)/sizeof(vertexes[0]) << endl;
    cout << "\nOrder of discovery: ";
    for (unsigned int i = 0; i <= vertexes[63].id; ++i)
    {
        cout << vertexes[discover_order[i]].id << " ";
    }
    cout << endl << endl;

    boost::print_graph(graph);
*/
//============================================================================//

//=========================GET BFS WORKING V2=================================//

    //edge = add_edge(vertexes[63].id, vertexes[63].id, EdgeWeightProperty(0), graph);
    Graph G_copy(num_vertices(graph));
    typedef Graph::vertex_descriptor Vertex;
    // The source vertex
    Vertex s = *(boost::vertices(graph).first);

    // Array to store predecessor (parent) of each vertex. This will be
    // used as a Decorator (actually, its iterator will be).
    std::vector<Vertex> p(boost::num_vertices(graph));

    // VC++ version of std::vector has no ::pointer, so
    // I use ::value_type* instead.
    typedef std::vector<Vertex>::value_type* Piter; //NEEDED??

    boost::graph_traits<Graph>::vertices_size_type d[totalVertices - 1];
    std::fill_n(d, totalVertices - 1, 0);

     p[s] = s;

/*
    //Call BFS
    boost::breadth_first_search
    (graph, s,
    boost::visitor(boost::make_bfs_visitor
    (std::make_pair(boost::record_distances(d, boost::on_tree_edge()),
                    std::make_pair
                    (boost::record_predecessors(&p[0], boost::on_tree_edge()),
                     copy_graph(G_copy, boost::on_examine_edge())))) ));
*/

    //Call BFS
    boost::breadth_first_search
    (graph, s,
    boost::visitor(boost::make_bfs_visitor
    (std::make_pair(boost::record_distances(d, boost::on_tree_edge()),
                    std::make_pair
                    (boost::record_predecessors(&p[0], boost::on_tree_edge()),
                     copy_graph(G_copy, boost::on_examine_edge())))) ));

    //Print it
    cout << endl;
    //boost::print_graph(graph);
    cout << endl;

    //boost::print_graph(G_copy);

    //cout << boost::num_vertices(graph);

    if (boost::num_vertices(graph) <= boost::num_vertices(graph))
    {
        /*
        std::cout << "distances: ";
        #ifdef BOOST_OLD_STREAM_ITERATORS
        std::copy(d, d + 5, std::ostream_iterator<int, char>(std::cout, " "));
        #else
        std::copy(d, d + 5, std::ostream_iterator<int>(std::cout, " "));
        #endif
        std::cout << std::endl;
        */
        std::for_each(boost::vertices(graph).first, boost::vertices(graph).second,
                     print_parent<Piter>(&p[0]));


        //cout << "End: " << boost::vertices(graph).end << endl;;

        //cout << "end: " << end << endl;


        /*for(unsigned int i = 0; i < predecessors.size(); i++)
        {
            if(predecessors[i] == 0)
                continue;
                pair<Vertex, int> tempPair;
                pair < map<Vertex, int>::iterator, bool > result;
                cout << "\nNode = " << predecessors[i]
                     << "\tParent = " << predecessors[i+1] << endl;

            tempPair.first = predecessors[i+1];
            vertexes[predecessors[i]].target.push_back(tempPair);
            cout << "Added sucesfully" << endl;
            cout << "Vertex: " << vertexes[predecessors[i]].id
                 << "\tParent: "
                 << vertexes[predecessors[i]].target[0].first
                 << endl;
            //<<vertexes[predecessors[i]].target.at(0)[predecessors[i+1]]
        }*/
    }

    // cout << "\nSource" << "\tTarget" << "\tWeight" << endl;
    for(tie(edge_it, edge_end) = boost::edges(graph); edge_it != edge_end; ++edge_it)
    {
        // cout << boost::source(*edge_it, graph) << "\t"
        //      << boost::target(*edge_it, graph) << "\t"
        //      << EdgeWeightMap[*edge_it] << endl;

        pair<Vertex, int> tempPair;
        // cout << "Adding: " << boost::target(*edge_it, graph)
        // << " to "
        // << vertexes[boost::source(*edge_it, graph)].id
        // << " with weight: "
        // << EdgeWeightMap[*edge_it] << endl;
        tempPair.first = boost::target(*edge_it, graph);
        tempPair.second = EdgeWeightMap[*edge_it];
        vertexes[boost::source(*edge_it, graph)].target.push_back(tempPair);

        // int vecSize = vertexes[boost::source(*edge_it, graph)].target.size();
        // cout << "Current vector size for node "
        // << vertexes[boost::source(*edge_it, graph)].id
        // << " is: " << vecSize << endl;
        // cout << "Added the following:"
        // << "\nNode: "
        // << vertexes[boost::source(*edge_it, graph)].id
        // << "\tTarget: "
        // << vertexes[boost::source(*edge_it, graph)].target[vecSize-1].first
        // << "\tEdge Weight: "
        // << vertexes[boost::source(*edge_it, graph)].target[vecSize-1].second
        // << endl << endl;
    }

    Vertex end;
    vector <Vertex> predecessors;
    for(end = vertex(totalRows*totalColumns - 1, graph); end != vertex(0, graph); end = p[end])
    {
        //cout << "end: " << end << endl;
        predecessors.push_back(end);
    }
    predecessors.push_back(end);

    reverse(predecessors.begin(), predecessors.end());
    for(unsigned int i = 0; i < predecessors.size(); i++)
    {
        /*
        if(predecessors[i] == 0)
        {
            continue;
        }
        */
        cout << "\nNode: " << predecessors[i]
             << " \t Target: " << predecessors[i+1] << endl;

        int vecSize;
        unsigned int targetVector;
        targetVector = predecessors[i+1];
        vecSize = vertexes[predecessors[i]].target.size();
        cout << "Vector Size: " << vecSize << endl;
        for(int j = 0; j < vecSize; j++)
        {
            if(vertexes[predecessors[i]].target[j].first == targetVector)
            {
                cout << vertexes[predecessors[i]].target[j].second
                     << vertexes[predecessors[i]].direction << endl;
                outFile << vertexes[predecessors[i]].target[j].second
                        << vertexes[predecessors[i]].direction << " ";
            }
        }
    }






//============================================================================//

//=========================GET BFS WORKING V3=================================//
/*
    struct my_null_visitor
    {
        typedef on_no_event event_filter;
        template <class T, class Graph>
        void operator()(T, Graph&) {}
    };

    //breadth_first_search(graph, vertexes[0].id, visitor(vis));


    vector<boost::graph_traits<Graph>::vertices_size_type> discover_order(vertexes[63].id);
    boost::integer_range < int >range(0, 64);
    copy(range.begin(), range.end(), discover_order.begin());

    cout << "\nOrder of discovery: ";
    for (unsigned int i = 0; i <= vertexes[63].id; ++i)
    {
        cout << vertexes[discover_order[i]].id << " ";
    }
*/
//============================================================================//











    //Define a new type for simplicity. This will be a vertex iterator type
    typedef boost::adjacency_list<>::vertex_iterator vertIterator;

    //vertices() returns a pair of iterators for begining and end of graph
    pair<vertIterator, vertIterator> vertPair = vertices(graph);

    //Loop through entire graph from first vertex id to end
    for (vertIterator it = vertPair.first; it != vertPair.second; ++it)
    {
        //cout << *it << endl;
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
