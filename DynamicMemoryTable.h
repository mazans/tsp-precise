#ifndef PEA_ETAP1_DYNAMICMEMORYTABLE_H
#define PEA_ETAP1_DYNAMICMEMORYTABLE_H

#include <utility>

using std::pair;

//Posluzy za tablice zapamietujaca czastkowe wyniki wyznaczania najkrotszej sciezki
//Klasa ta uwzglednia fakt ze zaczynamy odd wierzcholka o indeksie 0
//Oznacza to, ze nie bedzie zapamietywac sciezek dla tego wierzcholka
//Dodatkowo klasa ta zapamietuje poprzednika, wystepujacego przed ostatnim wierzcholkiem
//dla danego rozwiazanego podproblemu
//pomaga to potem w wyznaczeniu optymalnej trasy
class DynamicMemoryTable {
private:
    int vertices_amount;
    pair<int, int>** memory;
public:
    DynamicMemoryTable(int vertices_amount);
    ~DynamicMemoryTable();

    int getCost(int vertex, int mask);
    void setCost(int vertex, int mask, int cost);

    int getPreviousVertex(int vertex, int mask);
    void setPreviousVertex(int vertex, int mask, int previous_vertex);
};


#endif
