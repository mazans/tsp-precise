#include <climits>
#include <queue>
#include <iostream>
#include "TSPResolver.h"

// ********************************************** //
// Funkcje dla programowania dynamicznego
// ********************************************** //

//Glowna funkcja oparta na dynamicznym algorytmie Helda-Karpa
TSP_result TSPResolver::resolveUsingDynamicAlgorithm(Graph &graph) {
    TSP_result result {INT_MAX, vector<int>(graph.getVertixNumber()+1)};
    DynamicMemoryTable table(graph.getVertixNumber());

    firstPhase(table, graph);
    calculationPhase(table, graph);

    pair<int, int> find_min_cost_result;
    find_min_cost_result = findMinTour(table, graph);
    result.cost = find_min_cost_result.first;

    findOptimalTour(table, graph, result.path, find_min_cost_result.second);

    return result;
}

//Funkcja wykonująca pierwszą fazę algorytmu
//Wyznacza koszt przebycia komiwojazera z wierzcholka 0 do kazdego z pozostalych wierzcholkow
//Zapisuje ten koszt w tabeli
void TSPResolver::firstPhase(DynamicMemoryTable &table, Graph &graph) {
    for(int vertex = 1; vertex < graph.getVertixNumber(); vertex++) {
        table.setCost(vertex, 1 << (vertex-1), graph.getEdgeValue(0, vertex));
        table.setPreviousVertex(vertex, 1 << (vertex-1), 0);
    }
}

//Glowna faza algorytmu Helda-Karpa
void TSPResolver::calculationPhase(DynamicMemoryTable &table, Graph &graph) {
    //wyznaczenie kosztu przebycia do kazdego wierzcholka przy warunku przejscia przez visited_vertices_amount wierzcholkow
    for(int visited_vertices_amount = 2; visited_vertices_amount < graph.getVertixNumber(); visited_vertices_amount++) {
        //Wyznaczenie wszystkich kombinacji visited_vertices_amount elementowych
        list<int> combinations = generateCombinations(visited_vertices_amount, graph.getVertixNumber());

        //liczba calkowita set reprezentuje zbior odwiedzonych wierzcholkow
        //jesli na i-tej pozycji znajduje sie jeden, to i-ty wierzcholek znajduje sie w zbiorze (repr. binarna)
        //UWAGA!! pozycje w liczbie set sa liczone od 1, a nie od 0
        //np. liczba 01101 oznacza, ze odwiedzony zostal 1, 3 i 4 wierzcholek
        for(int set : combinations) {
            //Wyznazcenie trasy do kazdego wierzcholka w zbiorze set przez wierzcholki z tego zbioru
            for(int vertex = 1; vertex < graph.getVertixNumber(); vertex++) {
                if(contains(vertex, set)) {
                    int subset = set ^ 1 << vertex-1;
                    int min_distance = INT_MAX;
                    int previous_vertex_min = INT_MAX;
                    //Znalezienie przedostatniego wierzcholka i sprawdzenie czy trasa od niego do wierzcholka vertex
                    //jest lepsza od aktualnej
                    for(int previous_vertex = 1; previous_vertex < graph.getVertixNumber(); previous_vertex++) {
                        if(previous_vertex != vertex  && contains(previous_vertex, set)) {
                            int new_distance = table.getCost(previous_vertex, subset) +
                                    graph.getEdgeValue(previous_vertex, vertex);
                            if(new_distance < min_distance) {
                                min_distance = new_distance;
                                previous_vertex_min = previous_vertex;
                            }
                        }
                    }
                    table.setCost(vertex, set, min_distance);
                    table.setPreviousVertex(vertex, set, previous_vertex_min);
                }
            }
        }
    }
}

//Funkcja generujaca wszystkie mozliwe kombinacje zbiorow amount elementowych ze zbioru vertices_amount elementowego
list<int> TSPResolver::generateCombinations(int amount, int vertices_amount) {
    list<int> result;
    combinationsHelper(0, 1, amount, result, vertices_amount);
    return result;
}

void TSPResolver::combinationsHelper(int current_set, int current_vertex, int repetitions_left,
                                     list<int> &combinations, int vertices_amount) {
    if(repetitions_left == 0)
        combinations.push_back(current_set);
    else {
        for(int i = current_vertex; i < vertices_amount; i++) {
            current_set = current_set | (1 << (i-1));
            combinationsHelper(current_set, i + 1, repetitions_left - 1, combinations, vertices_amount);
            current_set = current_set & ~(1 << (i-1));
        }
    }
}

//funkcja sprawdza czy zadany wierzcholek zawiera sie w zbiorze
bool TSPResolver::contains(int vertex, int set) {
    return (1 << vertex-1 & set) != 0;
}

//funkcja zwraca najmniejszy koszt przejscia przez wszystkie wierzcholki i powrotu na koniec do wierzcholka 0
//oraz indeks wierzcholka bedacego przed wierzcholkiem 0
pair<int, int> TSPResolver::findMinTour(DynamicMemoryTable &table, Graph &graph){
    pair<int, int> result;
    result.first = INT_MAX;
    result.second = INT_MAX;
    //zbior wszystkich wierzcholkow
    int end_state = (1 << (graph.getVertixNumber()-1)) - 1;
    int travel_cost;

    //wyszukiwanie najmniejszego kosztu
    for(int vertex = 1; vertex < graph.getVertixNumber(); vertex++) {
        travel_cost = table.getCost(vertex, end_state) + graph.getEdgeValue(vertex, 0);
        if(travel_cost < result.first) {
            result.first = travel_cost;
            result.second = vertex;
        }
    }
    return result;
}

//funkcja zwraca optymalny cykl Hamiltona
void TSPResolver::findOptimalTour(DynamicMemoryTable &table, Graph &graph, vector<int> &result, int previous_vertex) {

    int current_vertex = previous_vertex;
    int temp_vertex;
    int current_state = (1 << graph.getVertixNumber()-1) - 1; //stan koncowy

    for(int i = graph.getVertixNumber()-1; i > 0; i--) {
        result[i] = current_vertex;
        temp_vertex = current_vertex;
        current_vertex = table.getPreviousVertex(current_vertex, current_state);
        current_state = current_state ^ (1 << (temp_vertex-1));
    }

    result[0] = result[graph.getVertixNumber()] = 0;
}

// ********************************************** //
// Funkcja dla metody podzialu i ograniczen
// ********************************************** //

TSP_result TSPResolver::resolveUsingBranchAndBound(Graph &graph) {
    TSP_result result;
    result.cost = INT_MAX;

    vector<int> path(graph.getVertixNumber() + 1);
    for(int i = 0; i < graph.getVertixNumber(); i++)
        path[i] = i;
    path[graph.getVertixNumber()] = 0;

    branchAndBoundHelper(path, 1, result.cost, result.path, graph);

    return result;
}

void TSPResolver::branchAndBoundHelper(vector<int> &path, int number, int &min_cost, vector<int> &best_path,
                                       Graph &graph) {

    if(number == graph.getVertixNumber()) {
        int cost = 0;
        for(int i = 0; i < graph.getVertixNumber(); i++)
            cost += graph.getEdgeValue(path[i], path[i+1]);
        if(cost < min_cost) {
            min_cost = cost;
            best_path = path;
        }
    }
    else {
        if(calculateBound(path, number-1, graph) < min_cost) {
            for(int i = number; i < graph.getVertixNumber(); i++) {
                swap(path[i], path[number]);
                branchAndBoundHelper(path, number + 1, min_cost, best_path, graph);
                swap(path[i], path[number]);
            }
        }
    }
}

int TSPResolver::calculateBound(vector<int> &path, int number, Graph &graph) {
    int bound = 0;

    for(int i = 0; i < number; i++)
        bound += graph.getEdgeValue(path[i], path[i+1]);

    int min = graph.getEdgeValue(path[number], path[number+1]);
    for(int i = number+2; i < graph.getVertixNumber(); i++) {
        if(graph.getEdgeValue(path[number], path[i]) < min)
            min = graph.getEdgeValue(path[number], path[i]);
    }
    bound += min;

    for(int i = number + 1; i < graph.getVertixNumber(); i++) {
        min = graph.getEdgeValue(path[i], 0);
        for(int j = number + 1; j < graph.getVertixNumber(); j++) {
            if(i != j && graph.getEdgeValue(path[i], path[j]) < min)
                min = graph.getEdgeValue(path[i], path[j]);
        }
        bound += min;
    }

    return bound;
}

// ********************************************** //
// Funkcje dla przegladu zupelnego
// ********************************************** //

TSP_result TSPResolver::resolveUsingBruteforce(Graph &graph) {
    TSP_result result;
    result.cost = INT_MAX;

    vector<int> path(graph.getVertixNumber() + 1);
    for(int i = 0; i < graph.getVertixNumber(); i++) {
        path[i] = i;
    }
    path[graph.getVertixNumber()] = 0;

    bruteforceHelper(path, graph.getVertixNumber(), result.cost, result.path, graph);

    return result;
}

void TSPResolver::bruteforceHelper(vector<int> &path, int number, int &min_cost, vector<int> &best_path, Graph &graph) {
    //jesli wszystkie permutacje zostaly dokonane, to obliczamy koszt sciezki i sprawdzamy
    //czy jest lepsza od aktualnej
    if(number == 1) {
        int cost = 0;
        for(int i = 0; i < graph.getVertixNumber(); i++)
            cost += graph.getEdgeValue(path[i], path[i + 1]);
        if(cost < min_cost) {
            min_cost = cost;
            best_path = path;
        }
    }
    else {
        int a, b;
        for(int i = 1; i < number; i++) {
            a = path[i];
            b = path[number-1];
            path[i] = b;
            path[number-1] = a;

            bruteforceHelper(path, number - 1, min_cost, best_path, graph);

            a = path[i];
            b = path[number-1];
            path[i] = b;
            path[number-1] = a;
        }
    }
}