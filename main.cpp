#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include "Graph.h"
#include "TSPResolver.h"

using namespace std;

void testGraphFromFile();
void testTimeExecution();

void showGraph(Graph &gr);
void showResult(TSP_result &result);

Graph generateGraph(int size);


int main() {

    char choice;

    while(true) {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Pobierz graf z pliku i wykonaj testy" << endl;
        cout << "2. Wykonaj test pomiaru czasu" << endl;
        cout << "X. Wyjdz" << endl;
        cin >> choice;
        if(choice == '1')
            testGraphFromFile();
        else if(choice == '2')
            testTimeExecution();
        else if(choice == 'X')
            break;
        system("clear");
    }
    return 0;
}

void testGraphFromFile() {
    string file_name;
    cout << "Podaj nazwe pliku z ktorego maja zostac pobrane dane: ";
    cin >> file_name;
    ifstream fin(file_name.c_str());

    int size;
    Graph graph;

    if(fin.is_open())
    {
        fin >> size;
        if(fin.fail())
        {
            cout << "Blad odczytu rozmiaru struktury!" << endl;;
            cin.get();
            cin.get();
            return;
        }
        else
        {
            graph = Graph(size);
            int val;
            for(int i = 0; i < size; i++)
            {
                for(int j = 0; j < size; j++)
                {
                    fin >> val;
                    if(fin.fail())
                    {
                        cout << "Blad w trakcie odczytu danych ze struktury!";
                        cin.get();
                        cin.get();
                        fin.close();
                        return;
                    }
                    else
                    {
                        graph.setEdgeValue(i, j, val);
                    }
                }
            }
            fin.close();
        }
    }
    else
    {
        cout << "Blad w trakcie otwierania pliku!" << endl;
        cin.get();
        cin.get();
        return;
    }
    while(true) {
        char choice;
        TSP_result result;
        showGraph(graph);
        cout << "Wybierz opcje: " << endl;
        cout << "1. Programowanie dynamiczne" << endl;
        cout << "2. Metoda podzialu i ograniczen" << endl;
        cout << "3. Przeglad zupelny" << endl;
        cout << "X. Wyjdz" << endl;
        cin >> choice;
        cin.get();
        if(choice == '1') {
            result = TSPResolver::resolveUsingDynamicAlgorithm(graph);
            cout << "Programowanie dynamiczne:" << endl;
            showResult(result);
        }
        else if(choice == '2') {
            result = TSPResolver::resolveUsingBranchAndBound(graph);
            cout << "Metoda podzialu i ograniczen:" << endl;
            showResult(result);
        }
        else if(choice == '3') {
            result = TSPResolver::resolveUsingBruteforce(graph);
            cout << "Przeglad zupelny:" << endl;
            showResult(result);
        }
        else if(choice == 'X') {
            break;
        }
        else {
            cout << "Nieprawidlowa opcja" << endl;
        }
        cin.get();
    }

}

void showGraph(Graph &gr) {
    cout << "Graf:" << endl;
    for (int i = 0; i < gr.getVertixNumber(); i++)
    {
        for(int j = 0; j < gr.getVertixNumber(); j++)
        {
            cout << setw(3) << gr.getEdgeValue(i, j);
            cout << " ";
        }
        cout << endl << endl;
    }
}

void showResult(TSP_result &result) {
    cout << "Optymalna dlugosc sciezki: " << result.cost << endl;
    cout << "Wierzcholki optymalnej scezki:" << endl;
    for(int vertix : result.path) {
        cout << "-" << vertix;
    }
    cout << endl;
}

void testTimeExecution() {
    int size_amount = 12;
    int brute_size_amount = 9;
    int graph_amount = 100;

    int graph_size[size_amount] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18};

    int graph_size_brute[brute_size_amount] = {4, 5, 6, 7 ,8 ,9 ,10, 11, 12};

    Graph gr;
    long long dynamic_execution_time, bb_execution_time, bruteforce_execution_time;

    typedef chrono::high_resolution_clock Clock;
    chrono::time_point<Clock> start, stop;
    for(int i = 0; i < size_amount; i++)
    {
        bb_execution_time = dynamic_execution_time = 0;
        for(int k = 0; k < graph_amount; k++) {
            gr = generateGraph(graph_size[i]);

            start = Clock::now();
            TSPResolver::resolveUsingDynamicAlgorithm(gr);
            stop = Clock::now();
            dynamic_execution_time +=  chrono::duration_cast<chrono::nanoseconds>(stop-start).count();

            start = Clock::now();
            TSPResolver::resolveUsingBranchAndBound(gr);
            stop = Clock::now();
            bb_execution_time +=  chrono::duration_cast<chrono::nanoseconds>(stop-start).count();
        }

        cout << "Wierzcholkow: " << graph_size[i] << endl;
        cout << "Czas wykonania(programowanie dynamiczne): " << dynamic_execution_time/graph_amount << endl;
        cout << "Czas wykonania(metoda podzialu i ograniczen): " << bb_execution_time/graph_amount << endl;
    }

    for(int i = 0; i < brute_size_amount; i++)
    {
        bruteforce_execution_time = 0;
        for(int k = 0; k < graph_amount; k++) {
            gr = generateGraph(graph_size_brute[i]);

            start = Clock::now();
            TSPResolver::resolveUsingBruteforce(gr);
            stop = Clock::now();
            bruteforce_execution_time +=  chrono::duration_cast<chrono::nanoseconds>(stop-start).count();
        }

        cout << "Wierzcholkow: " << graph_size_brute[i] << endl;
        cout << "Czas wykonania(przeglad zupelny): " << bruteforce_execution_time/graph_amount << endl;
    }
}

Graph generateGraph(int size) {
    Graph result(size);
    int scale;

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(i != j) {
                scale = rand() % 500 + 1;
                result.setEdgeValue(i, j, scale);
            }
        }
    }
    return result;
}