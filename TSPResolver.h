#ifndef PEA_ETAP1_TSPRESOLVER_H
#define PEA_ETAP1_TSPRESOLVER_H

#include <vector>
#include "Graph.h"
#include "DynamicMemoryTable.h"


using namespace std;

//wynik wykonywaia sie algorytmu szukajacego najlepszego cyklu dla problemu komiwojazera
struct TSP_result {
    int cost;
    vector<int> path;
};

class TSPResolver {
private:

    //funkcje pomocnicze dla programowania dynamicznego
    static void firstPhase(DynamicMemoryTable &table, Graph &graph);
    static void calculationPhase(DynamicMemoryTable &table, Graph &graph);
    static pair<int, int> findMinTour(DynamicMemoryTable &table, Graph &graph);
    static void findOptimalTour(DynamicMemoryTable &table, Graph &graph, vector<int> &result, int last_vertex);

    static list<int> generateCombinations(int amount, int vertices_amount);
    static void combinationsHelper(int current_set, int current_vertex, int repetitions_left,
                                   list<int> &combinations, int vertices_amount);
    static bool contains(int vertex, int set);

    static void branchAndBoundHelper(vector<int> &path, int number, int &min_cost, vector<int> &best_path, Graph &graph);

    static int calculateBound(vector<int> &path, int number, Graph & graph);

    //funckja pomocnicza dla przegladu zupelnego
    static void bruteforceHelper(vector<int> &path, int number, int &min_cost, vector<int> &best_path, Graph &graph);

public:
    static TSP_result resolveUsingDynamicAlgorithm(Graph &graph);
    static TSP_result resolveUsingBranchAndBound(Graph &graph);
    static TSP_result resolveUsingBruteforce(Graph &graph);
};


#endif