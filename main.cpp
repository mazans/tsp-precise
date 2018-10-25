#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include "Graph.h"
#include "TSPResolver.h"

using namespace std;

void test_graph_from_file();
void test_time_execution();

void show_graph(Graph* gr);
void show_result(TSP_result& result);

Graph generate_graph(int size);


int main() {

    char choice;

    while(true) {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Pobierz graf z pliku i wykonaj testy" << endl;
        cout << "2. Wykonaj test pomiaru czasu" << endl;
        cout << "X. Wyjdz" << endl;
        cin >> choice;
        if(choice == '1')
            test_graph_from_file();
        else if(choice == '2')
            test_time_execution();
        else if(choice == 'X')
            break;
        system("clear");
    }
    return 0;
}

void test_graph_from_file() {
    string file_name;
    cout << "Podaj nazwe pliku z ktorego maja zostac pobrane dane: ";
    cin >> file_name;
    ifstream fin(file_name.c_str());

    int size;
    Graph * graph;

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
            graph = new Graph(size);
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
                        graph->set_edge_value(i, j, val);
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
    show_graph(graph);
    TSP_result result;
    result = TSPResolver::resolve_using_dynamic_algorithm(*graph);
    cout << "Programowanie dynamiczne:" << endl;
    show_result(result);
    result = TSPResolver::resolve_using_bruteforce(*graph);
    cout << "Przeglad zupelny:" << endl;
    show_result(result);
    result = TSPResolver::resolve_using_branch_and_bound(*graph);
    cout << "Metoda podzialu i ograniczen:" << endl;
    show_result(result);
    cin.get();
    cin.get();
    delete graph;
}

void show_graph(Graph* gr) {
    cout << "Graf:" << endl;
    for (int i = 0; i < gr->get_vertix_number(); i++)
    {
        for(int j = 0; j < gr->get_vertix_number(); j++)
        {
            cout << setw(3) << gr->get_edge_value(i, j);
            cout << " ";
        }
        cout << endl << endl;
    }
}

void show_result(TSP_result& result) {
    cout << "Optymalna dlugosc sciezki: " << result.cost << endl;
    cout << "Wierzcholki optymalnej scezki:" << endl;
    for(int vertix : result.path) {
        cout << "-" << vertix;
    }
    cout << endl;
}

void test_time_execution() {
    int size_amount = 7;
    int graph_amount = 100;

    int graph_size[size_amount] = {5, 10, 15, 20, 25, 30, 35};

    Graph gr;
    long long execution_time;

    typedef chrono::high_resolution_clock Clock;
    chrono::time_point<Clock> start, stop;

    for(int i = 0; i < size_amount; i++)
    {
        execution_time = 0;
        for(int k = 0; k < graph_amount; k++) {
            gr = generate_graph(graph_size[i]);
            start = Clock::now();
            TSPResolver::resolve_using_dynamic_algorithm(gr);
            stop = Clock::now();
            execution_time +=  chrono::duration_cast<chrono::nanoseconds>(stop-start).count();
        }

        cout << "Wierzcholkow: " << graph_size[i] << endl;
        cout << "Czas wykonania: " << execution_time/graph_amount << endl;
    }
}

Graph generate_graph(int size) {
    Graph result(size);
    int scale;

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(i != j) {
                scale = rand() % 500 + 1;
                result.set_edge_value(i, j, scale);
            }
        }
    }
    return result;
}