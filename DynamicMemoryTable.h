#ifndef PEA_ETAP1_DYNAMICMEMORYTABLE_H
#define PEA_ETAP1_DYNAMICMEMORYTABLE_H

//Posluzy za tablice zapamietujaca czastkowe wyniki wyznaczania najkrotszej sciezki
//Klasa ta uwzglednia fakt ze zaczynamy odd wierzcholka o indeksie 0
//Oznacza to, ze nie bedzie zapamietywac sciezek dla tego wiercholka
class DynamicMemoryTable {
private:
    int vertices_amount;
    int** memory;
public:
    DynamicMemoryTable(int vertices_amount);
    ~DynamicMemoryTable();

    int getCost(int vertex, int mask);
    void setCost(int vertex, int mask, int cost);
};


#endif
