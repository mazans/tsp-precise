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

    Graph(const Graph & graph);
    Graph operator=(const Graph& graph);

    int getVertixNumber();
    void setEdgeValue(int from, int to, int value);
    int getEdgeValue(int from, int to);
};

#endif //PEA_1_GRAPH_H