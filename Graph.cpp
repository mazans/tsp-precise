#include <cmath>
#include <climits>
#include <list>
#include <iostream>
#include "Graph.h"

Graph::Graph() {
    vertices_amount = 1;
    matrix = new int*[vertices_amount];
    matrix[0] =new int[vertices_amount];
}

Graph::Graph(int verticesAmount) {
    this->vertices_amount = verticesAmount;
    matrix = new int *[verticesAmount];
    for(int i = 0; i < verticesAmount; i++){
        matrix[i] = new int[verticesAmount];
        for(int j = 0; j < verticesAmount; j++)
            matrix[i][j] = 0;
    }
}

Graph::Graph(const Graph & graph) {

    vertices_amount = graph.vertices_amount;
    matrix = new int *[vertices_amount];
    for(int i = 0; i < vertices_amount; i++){
        matrix[i] = new int[vertices_amount];
        for(int j = 0; j < vertices_amount; j++)
            matrix[i][j] = graph.matrix[i][j];
    }
}

Graph Graph::operator=(const Graph & graph) {
    if(this == &graph)
        return *this;

    for(int i = 0; i < vertices_amount; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;

    vertices_amount = graph.vertices_amount;
    matrix = new int *[vertices_amount];
    for(int i = 0; i < vertices_amount; i++){
        matrix[i] = new int[vertices_amount];
        for(int j = 0; j < vertices_amount; j++)
            matrix[i][j] = graph.matrix[i][j];
    }

    return *this;
}

int Graph::get_edge_value(int from, int to) {
    return matrix[from][to];
}

void Graph::set_edge_value(int from, int to, int value) {
    matrix[from][to] = value;
}

int Graph::get_vertix_number() {
    return vertices_amount;
}

Graph::~Graph() {
    for(int i = 0; i < vertices_amount; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;
}
