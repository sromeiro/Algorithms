#include<iostream>
#include<stdexcept>
#include<string>
#include<fstream>
#include<vector>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/breadth_first_search.hpp>

//Change file name here if needed.
#define FILENAME "input.txt"


//=============================TEMPLATES FOR BFS==============================//
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
    //Struct to save vertex information
    boost::adjacency_list<>::vertex_descriptor id;
    char color;
    string direction;

    //Vector of pairs that stores the target vertex and the edge weight to it
    vector <  pair<boost::adjacency_list<>::vertex_descriptor, int> > target;
};

int main(int argc, char* argv[])
{
    int totalRows, totalColumns, totalVertices;
    const char* fileName;

    //Define Edge Weight Property type
    typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
    //Define adjacency list directed graph type
    typedef boost:: adjacency_list< boost::listS,
                                    boost::vecS,
                                    boost::directedS,
                                    boost::no_property,
                                    EdgeWeightProperty > Graph;


    //Create graph
    Graph graph;

    pair<boost::adjacency_list<>::edge_descriptor, bool> edge;

    //Create Edge Weight Map to store edge weights
    boost::property_map < Graph, boost::edge_weight_t >::type EdgeWeightMap;
    EdgeWeightMap = get(boost::edge_weight, graph);

    //Edge iterators to iterate through all edges of graph
    boost::graph_traits< Graph >::edge_iterator edge_it, edge_end;

    //Check arguments passed to determine which input file to process
    if(argc == 1)
    {
        cout << "NOTE: Program can take one argument of <filename.txt>" << endl;
        cout << "As no arguments were passed, default file \"" << FILENAME
             << "\" will be used" << endl;
        fileName = FILENAME;
    }
    else if(argc == 2)
    {
        cout << "Filename \"" << argv[1] << "\" will be used" << endl;
        fileName = argv[1];
    }
    else
    {
        cout << "NOTE: Arguments provided exceeded limit of one argument."
             << " Please restart the program with one or no arguments passed."
             << endl;
        exit(0);
    }

    //Create the input file object
    ifstream inFile;
    inFile.open(fileName);
    if (!inFile)
    {
        // call system to stops
        cerr << "Unable to open file\n";
        exit(1);
    }

    //Create the output file object
    ofstream outFile;
    outFile.open("output.txt");
    if (!outFile)
    {
        // call system to stop
        cerr << "Unable to create file\n";
        exit(1);
    }

    //Extract maze dimensions
    inFile >> totalRows >> totalColumns;
    totalVertices = totalRows * totalColumns;

    //Create an array of vertex structures, an array of vertices
    vert vertexes[totalVertices];

    //Loop that adds vertices to graph and extracts their information
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
                //Skip if last vertex
                continue;
            }

            //Direction is N, E, S, W
            vertexes[i].direction = tempStr[2];
        }
    }


    //For loop to add correct edges between vertices
    for(int i = 0; i < totalVertices; i++)
    {
        //Variable j to keep track of vertex we're connecting an edge to
        int j;
        if(vertexes[i].direction.compare("E") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i; j < (totalColumns + i) - (i % totalColumns); j++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i; j < (totalColumns + i) - (i % totalColumns); j++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("W") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i; j >= (i/totalColumns) * totalColumns; j--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i; j >= (i/totalColumns) * totalColumns; j--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("N") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i; j >= 0; j -= totalColumns)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i; j >= 0; j -= totalColumns)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("S") == 0)
        {
            //variable edgeWeight for weight
            int edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i; j <= totalVertices - 1; j += totalColumns)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i; j <= totalVertices - 1; j += totalColumns)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if (vertexes[i].direction.compare("NW") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= totalColumns + 1, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i, x = i; j >= 0 && x >= (i/totalColumns) * totalColumns; j -= totalColumns + 1, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("SE") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i, x = i; j <= totalVertices - 1 && x < (totalColumns + i) - (i % totalColumns); j += totalColumns + 1, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);

                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i, x = i; j <= totalVertices && x < (totalColumns + i) - (i % totalColumns); j += totalColumns + 1, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                        else if(vertexes[j].color == 'O')
                        {
                            //Target FOUND add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("NE") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i, x = i; j >= 0 && x < (totalColumns + i) - (i % totalColumns); j -= totalColumns - 1, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i, x = i; j >= 0 && x < (totalColumns + i) - (i % totalColumns); j -= totalColumns - 1, x++)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
        else if(vertexes[i].direction.compare("SW") == 0)
        {
            //variable x to track horizontal movement, edgeWeight for weight
            int x, edgeWeight = -1;
            switch(vertexes[i].color)
            {
                case 'R':
                    //When we find a RED arrow
                    for(j = i, x = i; j <= totalVertices - 1 && x >= (i/totalColumns) * totalColumns; j += totalColumns - 1, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'B')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
                case 'B':
                    //When we find a BLUE arrow
                    for(j = i, x = i; j <= totalVertices - 1 && x >= (i/totalColumns) * totalColumns; j += totalColumns - 1, x--)
                    {
                        edgeWeight++;
                        if(vertexes[j].color == 'R')
                        {
                            //Add edge between i and j with weight
                            edge = add_edge(vertexes[i].id, vertexes[j].id, EdgeWeightProperty(edgeWeight), graph);
                        }
                    }
                    break;
            }
        }
    }


//==================================BFS SETUP=================================//
    //Needed for BFS traversal and predecessors
    Graph G_copy(num_vertices(graph));
    typedef Graph::vertex_descriptor Vertex;
    // The source vertex
    Vertex s = *(boost::vertices(graph).first);

    // Array to store predecessor (parent) of each vertex. This will be
    // used as a Decorator (actually, its iterator will be).
    std::vector<Vertex> p(boost::num_vertices(graph));

    boost::graph_traits<Graph>::vertices_size_type d[totalVertices - 1];
    std::fill_n(d, totalVertices - 1, 0);

     p[s] = s;

    //Call BFS
    boost::breadth_first_search
    (graph, s,
    boost::visitor(boost::make_bfs_visitor
    (std::make_pair(boost::record_distances(d, boost::on_tree_edge()),
                    std::make_pair
                    (boost::record_predecessors(&p[0], boost::on_tree_edge()),
                     copy_graph(G_copy, boost::on_examine_edge())))) ));

//============================================================================//

    //Loop through all edges/vertices and store the target and weights to source vertex
    for(tie(edge_it, edge_end) = boost::edges(graph); edge_it != edge_end; ++edge_it)
    {
        //Create a temp pair to store target and edge weight to add to vertex
        pair<Vertex, int> tempPair;
        tempPair.first = boost::target(*edge_it, graph);
        tempPair.second = EdgeWeightMap[*edge_it];

        //Add the stored info to the source vertex
        vertexes[boost::source(*edge_it, graph)].target.push_back(tempPair);
    }

    //Create vertex to iterate through predecessors
    Vertex end;
    vector <Vertex> predecessors;
    //Loop through vertices from end to start to find path and store predecessors
    for(end = vertex(totalRows*totalColumns - 1, graph); end != vertex(0, graph); end = p[end])
    {
        predecessors.push_back(end);
    }
    //Store the last predecessor that loop didn't catch
    predecessors.push_back(end);

    //predecessor vector is backwards, reverse it
    reverse(predecessors.begin(), predecessors.end());

    //Loop through all predecessors and print in requested format to output file
    for(unsigned int i = 0; i < predecessors.size(); i++)
    {
        int vecSize;
        unsigned int targetVector;
        targetVector = predecessors[i+1];
        vecSize = vertexes[predecessors[i]].target.size();
        for(int j = 0; j < vecSize; j++)
        {
            if(vertexes[predecessors[i]].target[j].first == targetVector)
            {
                outFile << vertexes[predecessors[i]].target[j].second
                        << vertexes[predecessors[i]].direction << " ";
            }
        }
    }

    //Close the open files
    inFile.close();
    outFile.close();

    //Successful termination of program
    cout << "\n\nSolution written to \"output.txt\"" << endl;
    cout << "\nProgram terminated sucesfully" << endl;
}
