#include <algorithm>
#include <iostream>
#include <iomanip>
#include <climits>
#include "Node.h"

Node::Node() {
    is_in_solution = new bool[1];
}


Node::Node(int graph_size){
    path = vector<int>({0});
    cost = INT_MAX;
    bound = INT_MAX;
    last_vertix = 0;
    this->graph_size = graph_size;

    is_in_solution = new bool[graph_size];
    for(int i = 0; i < graph_size; i++)
        is_in_solution[i] = false;
    is_in_solution[0] = true;

}

Node::Node(Node & parent, int new_vertix, Graph & graph) {
    this->path = parent.path;
    this->path.push_back(new_vertix);
    this->graph_size = parent.graph_size;
    this->last_vertix = new_vertix;

    this->is_in_solution = new bool[graph_size];
    for(int i = 0; i < graph_size; i++)
        this->is_in_solution[i] = parent.is_in_solution[i];
    this->is_in_solution[new_vertix] = true;

    calculateBound(graph);
    calculateCost(graph);
}

Node::Node(const Node &node) {
    path = node.path;
    cost = node.cost;
    bound = node.bound;
    last_vertix = node.last_vertix;
    graph_size = node.graph_size;

    is_in_solution =  new bool[graph_size];
    for(int i = 0; i < graph_size; i++)
        is_in_solution[i] = node.is_in_solution[i];

}

Node& Node::operator=(const Node & node) {
    if(this == &node)
        return *this;

    path = node.path;
    cost = node.cost;
    bound = node.bound;
    last_vertix = node.last_vertix;
    graph_size = node.graph_size;

    delete [] is_in_solution;

    is_in_solution =  new bool[graph_size];
    for(int i = 0; i < graph_size; i++)
        is_in_solution[i] = node.is_in_solution[i];

    return *this;
}

void Node::calculateBound(Graph &graph) {
    bound = 0;

    for(int i = 0; i < path.size() - 1; i++)
        bound += graph.getEdgeValue(path[i], path[i+1]);

    int min = INT_MAX;
    int edge_value;
    for(int i = 1; i < graph.getVertixNumber(); i++) {
        if(!is_in_solution[i]) {
            edge_value = graph.getEdgeValue(last_vertix, i);
            if(edge_value < min)
                min = edge_value;
        }
    }
    if(min != INT_MAX)
        bound += min;

    for(int i = 1; i < graph.getVertixNumber(); i++) {
        if(!is_in_solution[i]) {
            min = graph.getEdgeValue(i, 0);
            for(int j = 1; j < graph.getVertixNumber(); j++) {
                if(!is_in_solution[i]) {
                    edge_value = graph.getEdgeValue(i, j);
                    if(edge_value < min)
                        min = edge_value;
                }
            }
            bound += min;
        }
    }
}

void Node::calculateCost(Graph & graph) {
    if(path.size() == graph.getVertixNumber()) {
        cost = 0;
        for(int i = 0; i < graph.getVertixNumber() - 1; i++) {
            cost += graph.getEdgeValue(path[i], path[i+1]);
        }
        cost += graph.getEdgeValue(getLastVertix(), 0);
    }
    else
        cost = INT_MAX;
}

int Node::getCost() {
    return cost;
}

vector<int> & Node::getPath() {
    return path;
}

int Node::getBound() const {
    return bound;
}

int Node::getLastVertix() {
    auto it = path.end();
    int result = *(--it);
    return result;
}

bool operator<(const Node& first, const Node& second) {
    return first.getBound() > second.getBound();
}

//metoda zwraca wezly-potomkow danego wezla
list<Node> Node::getNodeChildren(Graph & gr, list<Node> &result) {
    for(int to = 1; to < gr.getVertixNumber(); to++){
        if(!is_in_solution[to]) {
            Node node(*this, to, gr);
            result.push_back(node);
        }
    }
    return result;
}

Node::~Node() {
    delete [] is_in_solution;
}


