#ifndef PEA_ETAP1_NODE_H
#define PEA_ETAP1_NODE_H

#include <list>
#include "Graph.h"

using namespace std;

class Node {
private:
    vector<int> path;
    int cost;
    int bound;
    int last_vertix;
    int graph_size;
    bool * is_in_solution;

    Node(Node & paernt, int new_vertix, Graph & graph); //konstruktor sluzacy do tworzenia dzeci-wezlow

    void calculateBound(Graph & graph);
    void calculateCost(Graph & graph);
public:
    Node();
    Node(int graph_size); //konstruktor wezla-rodzica
    ~Node();

    Node& operator=(const Node & node);
    Node(const Node & node);

    list<Node> getNodeChildren(Graph & graph, list<Node> &result);

    int getCost();
    int getBound() const;

    vector<int> & getPath();
    int getLastVertix();
};

bool operator<(const Node& first, const Node& second);

#endif
