#ifndef PEA_ETAP1_TSPRESOLVER_H
#define PEA_ETAP1_TSPRESOLVER_H

#include <vector>
#include "Graph.h"
#include "DynamicMemoryTable.h"
#include "Node.h"


using namespace std;

struct TSP_result {
    int cost;
    vector<int> path;
};

class TSPResolver {
private:
    static void first_phase(DynamicMemoryTable &table, Graph& graph);
    static void calculation_phase(DynamicMemoryTable &table, Graph& graph);
    static int find_min_cost(DynamicMemoryTable &table, Graph& graph);
    static vector<int> find_optimal_tour(DynamicMemoryTable &table, Graph& graph);

    static list<int> generate_combinations(int amount, int vertices_amount);
    static void combinations_helper(int current_set, int current_vertex, int repetitions_left,
                                    list<int>& combinations, int vertices_amount);
    static bool contains(int vertex, int set);

    static void calculate_child_node_cost(Node & current_node, Node & child_node);

    static void bruteforce_helper(vector<int>& path, int number, int& min_cost, vector<int>& best_path, Graph& graph);
public:
    static TSP_result resolve_using_dynamic_algorithm(Graph & graph);
    static TSP_result resolve_using_branch_and_bound(Graph & graph);
    static TSP_result resolve_using_bruteforce(Graph & graph);
};


#endif