#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QList>
#include <QVector>
#include <QDebug>


struct ControlPoint;
struct Vertex;
struct HalfEdge;

struct ControlPoint {
    double x;
    double y;
};

struct Vertex{

    double x;
    double y;
    HalfEdge* half;

};

struct HalfEdge {

    Vertex* vertex; //Vertice no qual a halfedge está chegando
    ControlPoint* controlpoint; //P1 para Vertex P0; P2 para Vertex P3

    HalfEdge* next;
    HalfEdge* prev;
    HalfEdge* twin;

    int cycle = 0;
};


class HalfEdgeGraph

{

public:

    QVector<QVector<double>> listadebezier;
    QVector<HalfEdge*> halfedgeslist;
    QVector<HalfEdge*> listadeciclos;
    QVector<HalfEdge*> linhassugestivas;
    QVector<HalfEdge*> linhasmembrosadjacentes;


    HalfEdge* outersketchcycle = nullptr;
    QVector <HalfEdge*> bordercycles;
    QVector <HalfEdge*> islandcycles;
    QVector <HalfEdge*> others;


    QVector <double> areas;
    QVector <int> cycle;


public:

    HalfEdgeGraph();
    HalfEdge* encontraHalfEdge (double x, double y);
    void geragrafodesemiarestas(QVector<QVector<double>> listadebezier);
    void imprimehalfedges();
    QVector<HalfEdge*> separaciclos();
    void classificaciclos();
    void defineciclo(HalfEdge* half);

    QVector<HalfEdge*> completacontornos ();
    void fechahalfedges(HalfEdge* linha1, HalfEdge* linha2);
    void identificapartessimetricas ();


};

#endif // HALFEDGE_H

