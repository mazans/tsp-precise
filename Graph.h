#ifndef PEA_1_GRAPH_H
#define PEA_1_GRAPH_H

#include <vector>
#include <list>
using namespace std;

//Klasa reprezenujaca graf
class Graph {
private:
    int vertices_amount;
    int ** matrix;
public:
    Graph();
    Graph(int verticesNumber);
    ~Graph();

    Graph operator=(const Graph& graph);

    int get_vertix_number();
    void set_edge_value(int from, int to, int value);
    int get_edge_value(int from, int to);
};

#endif //PEA_1_GRAPH_H