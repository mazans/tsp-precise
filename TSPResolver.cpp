//
// Created by sergio on 18.10.18.
//

#include <climits>
#include <queue>
#include <iostream>
#include "TSPResolver.h"

// ********************************************** //
// Funkcje dla programowania dynamicznego
// ********************************************** //

//Glowna funkcja oparta na dynamicznym algorytmie Helda-Karpa
TSP_result TSPResolver::resolve_using_dynamic_algorithm(Graph &graph) {
    TSP_result result;
    DynamicMemoryTable table(graph.get_vertix_number());

    first_phase(table, graph);
    calculation_phase(table, graph);
    result.cost = find_min_cost(table, graph);
    result.path = find_optimal_tour(table, graph);

    return result;
}

//Funkcja wykonująca pierwszą fazę algorytmu
//Wyznacza koszt przebycia komiwojazera z wierzcholka 0 do kzdego z pozostalych wierzcholkow
//Zapisuje ten koszt w tabeli
void TSPResolver::first_phase(DynamicMemoryTable &table, Graph& graph) {
    for(int vertex = 1; vertex < graph.get_vertix_number(); vertex++) {
        table.setCost(vertex, 1 << (vertex-1), graph.get_edge_value(0, vertex));
    }
}

//Glowna faza algorytmu Helda-Karpa
void TSPResolver::calculation_phase(DynamicMemoryTable &table, Graph& graph) {
    //wyznaczenie kosztu przebycia do kazdego wierzcholka przy warunku przejscia przez visited_vertices_amount wierzcholkow
    for(int visited_vertices_amount = 2; visited_vertices_amount < graph.get_vertix_number(); visited_vertices_amount++) {
        //Wyznaczenie wszystkich kombinacji visited_vertices_amount elementowych
        list<int> combinations = generate_combinations(visited_vertices_amount, graph.get_vertix_number());

        //liczba calkowita set reprezentuje zbior odwiedzonych wierzcholkow
        //jesli na i-tej pozycji znajduje sie jeden, to i-ty wierzcholek znajduje sie w zbiorze (repr. binarna)
        //UWAGA!! pozycje w liczbie set sa liczone od 1, a nie od 0
        //np. liczba 01101 oznacza, ze odwiedzony zostal 1, 3 i 4 wierzcholek
        for(int set : combinations) {
            //Wyznazcenie trasy do kazdego wierzcholka w zbiorze set przez wierzcholki z tego zbioru
            for(int vertex = 1; vertex < graph.get_vertix_number(); vertex++) {
                if(contains(vertex, set)) {
                    int subset = set ^ 1 << vertex-1;
                    int min_distance = INT_MAX;
                    //Znalezienie przedostatniego wierzcholka i sprawdzenie czy trasa od niego do wierzcholka vertex
                    //jest lepsza od aktualnej
                    for(int previous_vertex = 1; previous_vertex < graph.get_vertix_number(); previous_vertex++) {
                        if(previous_vertex != vertex  && contains(previous_vertex, set)) {
                            int new_distance = table.getCost(previous_vertex, subset) + graph.get_edge_value(previous_vertex, vertex);
                            if(new_distance < min_distance)
                                min_distance = new_distance;
                        }
                    }
                    table.setCost(vertex, set, min_distance);
                }
            }
        }
    }
}

//Funkcja generujaca wszystkie mozliwe kombinacje zbiorow amount elementowych ze zbioru vertices_amount elementowego
list<int> TSPResolver::generate_combinations(int amount, int vertices_amount) {
    list<int> result;
    combinations_helper(0, 1, amount, result, vertices_amount);
    return result;
}

void TSPResolver::combinations_helper(int current_set, int current_vertex, int repetitions_left,
                                      list<int> &combinations, int vertices_amount) {
    if(repetitions_left == 0)
        combinations.push_back(current_set);
    else {
        for(int i = current_vertex; i < vertices_amount; i++) {
            current_set = current_set | (1 << (i-1));
            combinations_helper(current_set, i+1, repetitions_left-1, combinations, vertices_amount);
            current_set = current_set & ~(1 << (i-1));
        }
    }
}

bool TSPResolver::contains(int vertex, int set) {
    return (1 << vertex-1 & set) != 0;
}

//funkcja zwraca najmniejszy koszt przejscia przez wszystkie wierzcholki i powrotu na koniec do wierzcholka 0
int TSPResolver::find_min_cost(DynamicMemoryTable &table, Graph& graph){
    int result = INT_MAX;
    //zbior wszystkich wierzcholkow
    int end_state = (1 << (graph.get_vertix_number()-1)) - 1;
    int travel_cost;

    //wyszukiwanie najmniejszego kosztu
    for(int i = 1; i < graph.get_vertix_number(); i++) {
        travel_cost = table.getCost(i, end_state) + graph.get_edge_value(i, 0);
        if(travel_cost < result)
            result = travel_cost;
    }
    return result;
}

vector<int> TSPResolver::find_optimal_tour(DynamicMemoryTable &table, Graph& graph) {
    vector<int> result(graph.get_vertix_number()+1);

    int old_distance, new_distance, previous_index;
    //zaczynamy szukanie naszej sciezki od konca, ostatni wierzcholek ma indeks 0
    int current_index = 0;
    int current_state = (1 << graph.get_vertix_number()-1) - 1;

    for(int i = graph.get_vertix_number()-1; i > 0; i--) {
        previous_index = -1;
        //szukamy tutaj wierzcholka, ktory w najkrotszej sciezce byl przed wiezcholkiem current_index
        for(int j = 1; j < graph.get_vertix_number(); j++) {
            if(contains(j, current_state)) {
                if(previous_index == -1)
                    previous_index = j;
                old_distance = table.getCost(previous_index, current_state) + graph.get_edge_value(previous_index, current_index);
                new_distance = table.getCost(j, current_state) + graph.get_edge_value(j, current_index);

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

    result[0] = result[graph.get_vertix_number()] = 0;

    return result;
}

// ********************************************** //
// Funkcje dla metody podzialu i ograniczen
// ********************************************** //

TSP_result TSPResolver::resolve_using_branch_and_bound(Graph & graph) {
    TSP_result result;

    Node parent_node(graph);

    priority_queue<Node> node_queue;
    node_queue.push(parent_node);

    Node best_node;

    int best_cost = INT_MAX;

    while(!node_queue.empty()) {
        Node current_node = node_queue.top();
        node_queue.pop();
        if(current_node.get_cost() < best_cost) {
            list<Node> children = current_node.get_node_children(graph.get_vertix_number());
            for(Node child_node : children) {
                if(child_node.get_amount_of_nodes_in_path() == graph.get_vertix_number() && child_node.get_cost() < best_cost) {
                    best_cost = child_node.get_cost();
                    best_node = child_node;
                }
                else if(child_node.get_cost() < best_cost)
                    node_queue.push(child_node);
            }
        }
    }
    for(int vertix : best_node.get_path())
        result.path.push_back(vertix);
    result.path.push_back(0);
    result.cost = best_cost;
    return result;
}

// ********************************************** //
// Funkcje dla przegladu zupelnego
// ********************************************** //

TSP_result TSPResolver::resolve_using_bruteforce(Graph &graph) {
    //Utworzenie sciezki 0,1,2,..n,0
    vector<int> path(graph.get_vertix_number()+1);
    for(int i = 0; i < graph.get_vertix_number(); i++) {
        path[i] = i;
    }
    path[graph.get_vertix_number()+1] = 0;

    TSP_result result;
    result.cost = INT_MAX;

    //funkcja bedzie wykonyac permutacje na przygotowanej sciezce, nie zmieniajac pozycji pierwszej i ostatniej
    //(gdyz musi byc to zawsze 0)
    bruteforce_helper(path, graph.get_vertix_number(), result.cost, result.path, graph);

    return result;
}

void TSPResolver::bruteforce_helper(vector<int>& path, int number, int& best_cost, vector<int>& best_path, Graph& graph) {
    if(number == 1) {
        int cost = 0;
        for(int i = 0; i < graph.get_vertix_number(); i++)
            cost += graph.get_edge_value(path[i], path[i+1]);
        if(cost < best_cost) {
            best_cost = cost;
            best_path = path;
        }
    }
    else {
        int a, b;
        for(int i = 1; i < number; i++) {
            a = path[i];
            b = path[number-1];
            path[number-1] = a;
            path[i] = b;

            bruteforce_helper(path, number-1, best_cost, best_path, graph);

            a = path[i];
            b = path[number-1];
            path[number-1] = a;
            path[i] = b;
        }
    }
}