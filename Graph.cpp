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
            matrix[i][j] = 0;
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

TSP_result Graph::tsk_dynamic() {

    //inicjalizacja tablicy zapamietujacej wyniki posrednich odleglosci
    //w tej macierzy pierwszy indeks i reprezentuje i+1 wierzcholek grafu
    int ** memory = new int*[vertices_amount-1];
    int length = pow(2, vertices_amount-1);
    for(int i = 0; i < vertices_amount-1; i++) {
        memory[i] = new int[length];
        for(int j = 0; j < length; j++) {
            memory[i][j] = INT_MAX;
        }
    }

    TSP_result result;

    first_phase(memory);
    calculation_phase(memory);
    result.cost = find_min_cost(memory);
    result.path = find_optimal_tour(memory);

    for(int i = 0; i < vertices_amount-1; i++) {
        delete [] memory[i];
    }
    delete [] memory;
    return result;
}

void Graph::first_phase(int **memory) {
    for(int vertex = 1; vertex < vertices_amount; vertex++) {
        memory[vertex-1][1 << (vertex-1)] = matrix[0][vertex];
    }
}

void Graph::calculation_phase(int **memory) {
    for(int visited_vertices_amount = 2; visited_vertices_amount < vertices_amount; visited_vertices_amount++) {
        list<int> combinations = generate_combinations(visited_vertices_amount);

        //liczba calkowita set reprezentuje zbior odwiedzonych wierzcholkow
        //jesli na i-tej pozycji znajduje sie jeden, to i-ty wierzcholek znajduje sie w zbiorze (repr. binarna)
        //UWAGA!! pozycje w liczbie set sa liczone od 1, a nie od 0
        for(int set : combinations) {
            for(int vertex = 1; vertex < vertices_amount; vertex++) {
                if(contains(vertex, set)) {
                    int subset = set ^ 1 << vertex-1;
                    int min_distance = INT_MAX;
                    for(int previous_vertex = 1; previous_vertex < vertices_amount; previous_vertex++) {
                        if(previous_vertex != vertex  && contains(previous_vertex, set)) {
                            int new_distance = memory[previous_vertex-1][subset] + matrix[previous_vertex][vertex];
                            if(new_distance < min_distance)
                                min_distance = new_distance;
                        }
                    }
                    memory[vertex-1][set] = min_distance;
                }
            }
        }
    }
}

list<int> Graph::generate_combinations(int amount) {
    list<int> result;
    combinations_helper(0, 1, amount, result);
    return result;
}


void Graph::combinations_helper(int current_set, int current_vertex, int repetitions_left,
                                list<int> &combinations) {
    if(repetitions_left == 0)
        combinations.push_back(current_set);
    else {
        for(int i = current_vertex; i < vertices_amount; i++) {
            current_set = current_set | (1 << (i-1));
            combinations_helper(current_set, i+1, repetitions_left-1, combinations);
            current_set = current_set & ~(1 << (i-1));
        }
    }
}

bool Graph::contains(int vertex, int set) {
    return (1 << vertex-1 & set) != 0;
}

int Graph::find_min_cost(int **memory) {
    int result = INT_MAX;
    int end_state = (1 << vertices_amount-1) - 1;
    int travel_cost;

    for(int i = 1; i < vertices_amount; i++) {
        travel_cost = memory[i-1][end_state] + matrix[i][0];
        if(travel_cost < result)
            result = travel_cost;
    }
    return result;
}


vector<int> Graph::find_optimal_tour(int **memory) {
    vector<int> result(vertices_amount+1);

    int old_distance, new_distance, previous_index;
    //zaczynamy szukanie naszej sciezki od konca, ostatni wierzcholek ma indeks 0
    int current_index = 0;
    int current_state = (1 << vertices_amount-1) - 1;

    for(int i = vertices_amount-1; i > 0; i--) {
        previous_index = -1;
        //szukamy tutaj wierzcholka, ktory w najkrotszej sciezce byl przed wiezcholkiem current_index
        for(int j = 1; j < vertices_amount; j++) {
            if(contains(j, current_state)) {
                if(previous_index == -1)
                    previous_index = j;
                old_distance = memory[previous_index-1][current_state] + matrix[previous_index][current_index];
                new_distance = memory[j-1][current_state] + matrix[j][current_index];

                if(old_distance > new_distance)
                    previous_index = j;
            }
        }

        //po wyznaczeniu wczesnieszego wierzcholka zapisujemy go w wektorze wynikow
        result[i] = previous_index;
        //dodakowo usuwamy wektor ze stanu wektorow do wyznaczenia sciezki
        current_state = current_state ^ (1 << (previous_index-1));
        current_index = previous_index;
    }

    result[0] = result[vertices_amount] = 0;

    return result;
}

Graph::~Graph() {
    for(int i = 0; i < vertices_amount; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;
}
