#ifndef PEA_1_GRAPH_H
#define PEA_1_GRAPH_H

#include <vector>
#include <list>
using namespace std;

struct TSP_result {
    int cost;
    vector<int> path;
};

class Graph {
private:
    int vertices_amount;
    int ** matrix;

    void first_phase(int** memory);
    void calculation_phase(int** memory);
    int find_min_cost(int** memory);
    vector<int> find_optimal_tour(int ** memory);

    list<int> generate_combinations(int amount);
    void combinations_helper(int current_set, int current_vertex, int repetitions_left, list<int>& combinations);
    bool contains(int vertex, int set);
public:
    Graph();
    Graph(int verticesNumber);
    ~Graph();

    Graph operator=(const Graph& graph);

    int get_vertix_number();
    void set_edge_value(int from, int to, int value);
    int get_edge_value(int from, int to);
    TSP_result tsk_dynamic();
};

#endif //PEA_1_GRAPH_H