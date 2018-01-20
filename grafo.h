#ifndef GRAFO_H
#define GRAFO_H

#include<iostream>
#include<list>
#include<QDebug>
#include<QVector>


using namespace std;

// Graph class represents a directed graph using adjacency list representation
class Grafo
{
    int V;    // No. of vertices
    list<int> *adj;    // Pointer to an array containing adjacency lists

    void DFSUtil(int v, bool visited[]);   // A function used by DFS


public:
    Grafo(int V);   // Constructor
    void adicionarAresta(int v, int w);   // function to add an edge to graph
    void DFS(int v);    // DFS traversal of the vertices reachable from v

    QVector<int> visitadoslist;
};


#endif // GRAFO_H
