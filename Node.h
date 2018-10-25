#ifndef PEA_ETAP1_NODE_H
#define PEA_ETAP1_NODE_H

#include <list>
#include "ResultCostMatrix.h"

using namespace std;

class Node {
private:
    ResultCostMatrix node_rcm;
    vector<int> path;
    int cost;
public:
    Node();
    ~Node() {}
    Node(vector<int>& path, ResultCostMatrix & rcm, int cost); //konstruktor sluzacy do tworzenia dzeci-wezlow
    Node(Graph & gr); //konstruktor wezla-rodzica

    list<Node> get_node_children(int vertices_amount);

    void set_cost(int cost);
    int get_cost() const;

    vector<int> & get_path();
    int get_amount_of_nodes_in_path();

    void set_result_cost_matrix(ResultCostMatrix & rcm);
    ResultCostMatrix & get_result_cost_matrix();

    int get_last_vertix();

    void show_Node();
};

bool operator<(const Node& first, const Node& second);

#endif
