#include <climits>
#include "ResultCostMatrix.h"

ResultCostMatrix::ResultCostMatrix() {
    vertices_amount = 0;
    memory = new int*[1];
}

ResultCostMatrix::ResultCostMatrix(Graph &graph) {
    this->vertices_amount = graph.get_vertix_number();
    memory = new int*[vertices_amount];
    for(int i = 0; i < vertices_amount; i++) {
        memory[i] = new int[vertices_amount];
        for(int j = 0; j < vertices_amount; j++) {
            if(i != j)
                memory[i][j] = graph.get_edge_value(i, j);
            else
                memory[i][j] = INT_MAX;
        }
    }
}

ResultCostMatrix::ResultCostMatrix(const ResultCostMatrix & rcm) {
    vertices_amount = rcm.vertices_amount;
    memory = new int *[vertices_amount];
    for(int i = 0; i < vertices_amount; i++){
        memory[i] = new int[vertices_amount];
        for(int j = 0; j < vertices_amount; j++)
            memory[i][j] = rcm.memory[i][j];
    }
}

ResultCostMatrix & ResultCostMatrix::operator=(const ResultCostMatrix & rs) {
    if(this == &rs)
        return *this;

    for(int i = 0; i < vertices_amount; i++) {
        delete [] memory[i];
    }
    delete [] memory;

    vertices_amount = rs.vertices_amount;
    memory = new int *[vertices_amount];
    for(int i = 0; i < vertices_amount; i++){
        memory[i] = new int[vertices_amount];
        for(int j = 0; j < vertices_amount; j++)
            memory[i][j] = rs.memory[i][j];
    }

    return *this;
}

int ResultCostMatrix::reduce_matrix() {
    int result = 0;
    int min_cost;
    bool only_infinity, has_zero;
    for(int i = 0; i < vertices_amount; i++) {
        only_infinity = true;
        has_zero =false;
        min_cost = INT_MAX;
        for(int j = 0; j < vertices_amount; j++) {
            if(memory[i][j] == 0) {
                has_zero = true;
                break;
            }
            if(memory[i][j] != INT_MAX)
                only_infinity = false;
            if(memory[i][j] < min_cost)
                min_cost = memory[i][j];
        }
        if(!only_infinity && !has_zero) {
            result += min_cost;
            for(int j = 0; j < vertices_amount; j++) {
                if(memory[i][j] != INT_MAX)
                    memory[i][j] = memory[i][j] - min_cost;
            }
        }
    }

    for(int i = 0; i < vertices_amount; i++) {
        only_infinity = true;
        has_zero =false;
        min_cost = INT_MAX;
        for(int j = 0; j < vertices_amount; j++) {
            if(memory[j][i] == 0) {
                has_zero = true;
                break;
            }
            if(memory[j][i] != INT_MAX)
                only_infinity = false;
            if(memory[j][i] < min_cost)
                min_cost = memory[j][i];
        }
        if(!only_infinity && !has_zero) {
            result += min_cost;
            for(int j = 0; j < vertices_amount; j++) {
                if(memory[j][i] != INT_MAX)
                    memory[j][i] = memory[j][i] - min_cost;
            }
        }
    }
    return result;
}

void ResultCostMatrix::remove_vertix(int from, int to) {
    for(int i = 0; i < vertices_amount; i++){
        memory[from][i] = INT_MAX;
        memory[i][to] = INT_MAX;
    }
    memory[to][0] = INT_MAX;
}

ResultCostMatrix::~ResultCostMatrix() {
    for(int i = 0; i < vertices_amount; i++) {
        delete [] memory[i];
    }
    delete [] memory;
}

int ResultCostMatrix::getCost(int from, int to) {
    return memory[from][to];
}

int ResultCostMatrix::get_vertix_number() {
    return vertices_amount;
}
