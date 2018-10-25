#include <algorithm>
#include <iostream>
#include <iomanip>
#include <climits>
#include "Node.h"

Node::Node() {
    path.push_back(0);
    cost = -1;
}

Node::Node(Graph & gr){
    path.push_back(0);
    node_rcm = ResultCostMatrix(gr);
    cost = node_rcm.reduce_matrix();
}

Node::Node(vector<int> &path, ResultCostMatrix &rcm, int cost) {
    this->path = path;
    this->node_rcm = rcm;
    this->cost = cost;
}

void Node::set_cost(int cost) {
    this->cost = cost;
}

int Node::get_cost() const {
    return cost;
}

vector<int> & Node::get_path() {
    return path;
}

int Node::get_amount_of_nodes_in_path() {
    return path.size();
}

bool operator<(const Node& first, const Node& second) {
    return first.get_cost() > second.get_cost();
}

list<Node> Node::get_node_children(int verticesAmount) {
    list<Node> result;
    int from = get_last_vertix();
    int child_cost;
    for(int to = 1; to < verticesAmount; to++){
        if(find(path.begin(), path.end(), to) == path.end()) {
            vector<int> child_path = path;
            child_path.push_back(to);

            ResultCostMatrix child_rcm = node_rcm;
            child_rcm.remove_vertix(from, to);
            int additional_cost = child_rcm.reduce_matrix();

            child_cost = cost + additional_cost + node_rcm.getCost(from, to);

            Node node(child_path, child_rcm, child_cost);
            result.push_back(node);
        }
    }
    return result;
}

void Node::set_result_cost_matrix(ResultCostMatrix & rcm) {
    node_rcm = rcm;
}

ResultCostMatrix &Node::get_result_cost_matrix() {
    return node_rcm;
}

int Node::get_last_vertix() {
    vector<int>::iterator it = path.end();
    int result = *(--it);
    return result;
}