#include <climits>
#include "DynamicMemoryTable.h"
#include <cmath>

//Macierz przechowuje koszta przejscia od wierzcholka 0 do danego wierzcholka, przy przejsciu przez konkretne wierzcholki
//Pierwszy indeks oznacza wierzcholek koncowy, drugi zbior wierzcholkow odwiedzonych na danej trasie
//Drugi indeks nalezy odczytac w reprezentacji binarnej w ceu sprawdzenia odwiedzonych wierzcholkow
//np. memory[3][7] oznacza koszt dotarcia do wierzcholka 3 od wierzcholka 0, przy warunku odwiedzenia wierzcholkow 1, 2 i 3
//(7 binarnie to 0111, to oznacza, ze odwiedzone zostaly wierzcholki 1, 2 i 3)
DynamicMemoryTable::DynamicMemoryTable(int vertices_amount)
{
    this->vertices_amount = vertices_amount;
    memory = new pair<int, int>*[vertices_amount-1];
    int length = pow(2, vertices_amount-1);
    for(int i = 0; i < vertices_amount-1; i++) {
        memory[i] = new pair<int, int>[length];
        for(int j = 0; j < length; j++) {
            //pierwszy element zapamietuje optymalna wartosc rozwiazania danego podproblemu
            //drugi element zapamietuje poprzednika ostatniego wierzcholka w danym podproblemie
            memory[i][j].first = INT_MAX;
            memory[i][j].first = INT_MAX;
        }
    }
}

void DynamicMemoryTable::setCost(int vertex, int mask, int cost) {
    memory[vertex-1][mask].first = cost;
}

int DynamicMemoryTable::getCost(int vertex, int mask) {
    return memory[vertex-1][mask].first;
}

void DynamicMemoryTable::setPreviousVertex(int vertex, int mask, int previous_vertex) {
    memory[vertex-1][mask].second = previous_vertex;
}

int DynamicMemoryTable::getPreviousVertex(int vertex, int mask) {
    return memory[vertex-1][mask].second;
}

DynamicMemoryTable::~DynamicMemoryTable()
{
    for(int i = 0; i < vertices_amount-1; i++) {
        delete [] memory[i];
    }
    delete [] memory;
}


