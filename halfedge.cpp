#include "halfedge.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <QtMath>

using namespace std;

HalfEdgeGraph::HalfEdgeGraph()
{

}

HalfEdge* HalfEdgeGraph::encontraHalfEdge (double x, double y){

    for (int j = 0;  j < halfedgeslist.size() ; ++j){
        if ((halfedgeslist[j]->vertex->x == x && halfedgeslist[j]->vertex->y == y )|| (halfedgeslist[j]->twin->vertex->x == x && halfedgeslist[j]->twin->vertex->y == y)){
            return halfedgeslist[j];
        }
    }
    return nullptr;
}

void HalfEdgeGraph::geragrafodesemiarestas(QVector<QVector<double>> listadebezier){
    HalfEdge *firstHE = new HalfEdge;
    HalfEdge *firstHEo = new HalfEdge;

    Vertex *vertex = new Vertex;
    ControlPoint *controlPoint = new ControlPoint;

    //Vértice que a Half-Edge está chegando
    vertex->x = listadebezier[0][6];
    vertex->y = listadebezier[0][7];

    controlPoint->x = listadebezier[0][2];
    controlPoint->y = listadebezier[0][3];

    firstHE->vertex = vertex;
    firstHE->controlpoint = controlPoint;
    firstHE->next = nullptr;
    firstHE->prev = nullptr;

    vertex = new Vertex;
    controlPoint = new ControlPoint;
    //Vértice que a Half-Edge está chegando (OPOSTA)

    vertex->x = listadebezier[0][0];
    vertex->y = listadebezier[0][1];

    controlPoint->x = listadebezier[0][4];
    controlPoint->y = listadebezier[0][5];

    firstHEo->vertex = vertex;
    firstHEo->controlpoint = controlPoint;

    firstHE->twin = firstHEo;
    firstHEo->twin = firstHE;
    firstHEo->next = nullptr;
    firstHEo->prev = nullptr;

    HalfEdge *he0 = firstHE;
    HalfEdge *he1;

    HalfEdge *he2 = firstHEo;
    HalfEdge *he3;

    //listadeciclos.push_back(firstHE);
    halfedgeslist.push_back(firstHE);


    for (int i = 1; i < listadebezier.size(); i++) {

        // CASO 1 - Os 2 endpoints já existem no grafo.. Fecha Ciclo
        // CASO 2 - Só o primeiro endpoint existe no grafo..
        // CASO 3 - Só o segundo endpoint existe no grafo..
        // CASO 4 - É um novo ciclo ou grafo

        he1 = new HalfEdge;
        he3 = new HalfEdge;

        int caso = 0;
        HalfEdge *half1;
        HalfEdge *half2;

        //half1 = findHalfEdge(listadebezier[i][6], listadebezier[i][7], listadeciclos);
        //half2 = findHalfEdge(listadebezier[i][0], listadebezier[i][1], listadeciclos);

        half1 = encontraHalfEdge(listadebezier[i][6], listadebezier[i][7]);
        half2 = encontraHalfEdge(listadebezier[i][0], listadebezier[i][1]);



        cout << "i = "<< i<<endl;

        cout << listadebezier[i][0] <<"," << listadebezier[i][1] <<" -> " << listadebezier[i][6]<<","<< listadebezier[i][7] <<endl;

        if (half1 != nullptr && half2 != nullptr){
            caso = 1;
            halfedgeslist.push_back(he1);

        }else if (half2 != nullptr && half1 == nullptr){
            caso = 2;
            halfedgeslist.push_back(he1);
        }else if (half1 != nullptr && half2 == nullptr){
            caso = 3;
            halfedgeslist.push_back(he1);
        } else {
            caso = 4;
        }

        switch (caso) {

        case 1:
            cout << "Caso: 1" << endl;

            if (half2->next == nullptr){

                vertex = new Vertex;
                controlPoint = new ControlPoint;

                //Vértice no qual a Half-Edge está chegando
                vertex->x = listadebezier[i][6];
                vertex->y = listadebezier[i][7];
                he1->vertex = vertex;
                controlPoint->x = listadebezier[i][2];
                controlPoint->y = listadebezier[i][3];
                he1->controlpoint = controlPoint;

                //Vértice no qual a Half-Edge está chegando (OPOSTA)
                vertex = new Vertex;
                controlPoint = new ControlPoint;

                vertex->x = listadebezier[i][0];
                vertex->y = listadebezier[i][1];
                he3->vertex = vertex;
                controlPoint->x = listadebezier[i][4];
                controlPoint->y = listadebezier[i][5];
                he3->controlpoint = controlPoint;

                he0->next = he1;
                he1->prev = he0;

                he2->prev = he3;
                he3->next = he2;

                he0->twin = he2;
                he2->twin = he0;

                he1->twin = he3;
                he3->twin = he1;

                he0 = he1;
                he2 = he3;

                /*he1->next = firstHE;
                    firstHE->prev = he1;
                    he3->prev = firstHEo;
                    firstHEo->next = he3;*/

                if (half2->prev == nullptr || half1->prev == nullptr) {

                    he1->next = firstHE;
                    firstHE->prev = he1;
                    he3->prev = firstHEo;
                    firstHEo->next = he3;
                } else {

                    he1->next = half1->next;
                    half1->next->prev = he1;
                    half1->next = he3;
                    he3->prev = half1;

                }


                cout << "Fechando ciclo, ligando " << he3->vertex->x <<","<< he3->vertex->y << "à" << he1->vertex->x <<"," <<he1->vertex->y <<endl;

                //firstHE = new HalfEdge;
                //firstHEo = new HalfEdge;


                // Neste caso, está fechando um ciclo com uma linha que já está aberta. (linha perna do gato)
            } else if (half1->next == half1->twin || half2->next == half2->twin){
                cout << "Ligando: " << half1->vertex->x <<"," <<half1->vertex->y << "à" << half2->vertex->x << "," << half2->vertex->y << endl;


                if (half2->next == half2->twin && half2->vertex->x == listadebezier[i][6]){

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    //Vértice no qual a Half-Edge está chegando
                    vertex->x = listadebezier[i][6];
                    vertex->y = listadebezier[i][7];
                    he1->vertex = vertex;
                    controlPoint->x = listadebezier[i][2];
                    controlPoint->y = listadebezier[i][3];
                    he1->controlpoint = controlPoint;

                    //Vértice no qual a Half-Edge está chegando (OPOSTA)
                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    vertex->x = listadebezier[i][0];
                    vertex->y = listadebezier[i][1];
                    he3->vertex = vertex;
                    controlPoint->x = listadebezier[i][4];
                    controlPoint->y = listadebezier[i][5];
                    he3->controlpoint = controlPoint;

                    he1->prev = half1->next->twin;
                    half1->next->twin->next = he1;

                    he3->next = half1->twin;
                    half1->twin->prev = he3;

                    he1->next = half2->twin;
                    half2->twin->prev = he1;

                    he3->prev = half1;
                    half1->next = he3;

                    he1->twin = he3;
                    he3->twin = he1;
                    //Linha desenhada ao contrário
                } else if (half2->next == half2->twin && half2->vertex->x == listadebezier[i][0]) {

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    //Vértice no qual a Half-Edge está chegando
                    vertex->x = listadebezier[i][0];
                    vertex->y = listadebezier[i][1];
                    he3->vertex = vertex;
                    controlPoint->x = listadebezier[i][4];
                    controlPoint->y = listadebezier[i][5];
                    he3->controlpoint = controlPoint;

                    //Vértice no qual a Half-Edge está chegando (OPOSTA)
                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    vertex->x = listadebezier[i][6];
                    vertex->y = listadebezier[i][7];
                    he1->vertex = vertex;
                    controlPoint->x = listadebezier[i][2];
                    controlPoint->y = listadebezier[i][3];
                    he1->controlpoint = controlPoint;

                    he3->prev = half1->next->twin;
                    half1->next->twin->next = he3;

                    he1->next = half1->twin;
                    half1->twin->prev = he1;

                    he3->next = half2->twin;
                    half2->twin->prev = he3;

                    he1->prev = half2;
                    half2->next = he1;

                    he1->twin = he3;
                    he3->twin = he1;
                }

            }

            else {

                if (half2->twin->vertex->x == listadebezier[i][0] && half1->vertex->x == listadebezier[i][6]) {
                    cout << "Fechando novo ciclo dentro de existente" << endl;
                    cout << "Ligando: " << listadebezier[i][0] << "," << listadebezier[i][1] << " a "
                         << half2->twin->vertex->x << "," << half2->twin->vertex->y << endl;
                    cout << "E : " << listadebezier[i][6] << "," << listadebezier[i][7] << " a " << half1->vertex->x
                         << "," << half1->vertex->y << endl;
                    //ControlPoint P1 - Existente e nova
                    double p1xexis = half2->controlpoint->x;
                    double p1yexis = half2->controlpoint->y;
                    double p1xnova = listadebezier[i][2];
                    double p1ynova = listadebezier[i][3];

                    double p3xvertice = half2->twin->next->controlpoint->x;
                    double p3yvertice = half2->twin->next->controlpoint->y;
                    double p0x = half2->twin->vertex->x;
                    double p0y = half2->twin->vertex->y;

                    double v1x = p1xexis - p0x;
                    double v1y = p1yexis - p0y;

                    double v2x = p3xvertice - p0x;
                    double v2y = p3yvertice - p0y;
                    double dot = (v1x * v2x) + (v1y * v2y);
                    double det = v1x * v2y - v1y * v2x;
                    double normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomaexistente = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomaexistente = 360 - angulocomaexistente;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }

                    cout << "Angulo com a EXISTENTE: " << angulocomaexistente << endl;

                    v1x = p1xnova - p0x;
                    v1y = p1ynova - p0y;
                    dot = (v1x * v2x) + (v1y * v2y);
                    det = v1x * v2y - v1y * v2x;
                    normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomanova = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomanova = 360 - angulocomanova;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }
                    cout << "Angulo com a NOVA: " << angulocomanova << endl;


                    if (angulocomanova < angulocomaexistente) {


                        vertex = new Vertex;
                        controlPoint = new ControlPoint;
                        // Link the from-side of the edge.
                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;
                        he1->prev = half1->prev;
                        he1->next = half2;
                        half2->prev = he1;
                        half1->prev->next = he1;

                        //Buscar--next da he1
                        //he1->next = half->next;

                        // Link the to-side of the edge.
                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;
                        he3->next = half1;

                        half1->prev = he3;

                        he3->prev = half1;

                        //Buscar--prev da he3

                        he1->twin = he3;
                        he3->twin = he1;


                    } else {
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;
                        he1->next = half1->twin;
                        half1->twin->prev = he1;
                        he1->prev = half2->twin;
                        half2->twin->next = he1;


                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;
                        he3->prev = half1->next->twin;
                        he3->next = half2->prev->twin;
                        half2->prev->twin->prev = he3;
                        half1->next->twin->next = he3;

                        he1->twin = he3;
                        he3->twin = he1;


                    }
                } else if (half2->next->vertex->x == listadebezier[i][0] && half1->vertex->x == listadebezier[i][6]) {

                    cout << "Fechando novo ciclo dentro de existente" << endl;
                    cout << "Ligando: " << listadebezier[i][0] << "," << listadebezier[i][1] << " a "
                         << half2->next->vertex->x << "," << half2->next->vertex->y << endl;
                    cout << "E : " << listadebezier[i][6] << "," << listadebezier[i][7] << " a " << half1->vertex->x
                         << "," << half1->vertex->y << endl;
                    //ControlPoint P1 - Existente e nova
                    double p1xexis = half2->next->controlpoint->x;
                    double p1yexis = half2->next->controlpoint->y;
                    double p1xnova = listadebezier[i][2];
                    double p1ynova = listadebezier[i][3];

                    double p3xvertice = half2->twin->controlpoint->x;
                    double p3yvertice = half2->twin->controlpoint->y;
                    double p0x = half2->vertex->x;
                    double p0y = half2->vertex->y;

                    double v1x = p1xexis - p0x;
                    double v1y = p1yexis - p0y;

                    double v2x = p3xvertice - p0x;
                    double v2y = p3yvertice - p0y;
                    double dot = (v1x * v2x) + (v1y * v2y);
                    double det = v1x * v2y - v1y * v2x;
                    double normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomaexistente = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomaexistente = 360 - angulocomaexistente;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }

                    cout << "Angulo com a EXISTENTE: " << angulocomaexistente << endl;

                    v1x = p1xnova - p0x;
                    v1y = p1ynova - p0y;
                    dot = (v1x * v2x) + (v1y * v2y);
                    det = v1x * v2y - v1y * v2x;
                    normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomanova = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomanova = 360 - angulocomanova;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }
                    cout << "Angulo com a NOVA: " << angulocomanova << endl;


                    if (angulocomanova < angulocomaexistente) {

                        vertex = new Vertex;
                        controlPoint = new ControlPoint;
                        // Link the from-side of the edge.
                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;
                        he1->prev = half1->prev;
                        he1->next = half2;
                        half2->prev = he1;
                        half1->prev->next = he1;

                        //Buscar--next da he1
                        //he1->next = half->next;

                        // Link the to-side of the edge.
                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;
                        he3->next = half1;

                        half1->prev = he3;

                        he3->prev = half1;

                        //Buscar--prev da he3

                        he1->twin = he3;
                        he3->twin = he1;


                    } else {
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;

                        he1->next = half1->twin;
                        half1->twin->prev = he1;
                        he1->prev = half2->next->twin;
                        half2->next->twin->next = he1;


                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;

                        he3->prev = half1->next->twin;
                        he3->next = half2->twin;
                        half2->twin->prev = he3;
                        half1->next->twin->next = he3;

                        he1->twin = he3;
                        he3->twin = he1;

                    }
                } else if (half2->vertex->x == listadebezier[i][0] && half1->vertex->x == listadebezier[i][6]){

                    cout << "Fechando novo ciclo dentro de existente" << endl;
                    cout << "Ligando: " << listadebezier[i][0] << "," << listadebezier[i][1] << " a "
                         << half2->vertex->x << "," << half2->vertex->y << endl;
                    cout << "E : " << listadebezier[i][6] << "," << listadebezier[i][7] << " a " << half1->vertex->x
                         << "," << half1->vertex->y << endl;
                    //ControlPoint P1 - Existente e nova
                    double p1xexis = half2->next->controlpoint->x;
                    double p1yexis = half2->next->controlpoint->y;
                    double p1xnova = listadebezier[i][2];
                    double p1ynova = listadebezier[i][3];

                    double p3xvertice = half2->twin->controlpoint->x;
                    double p3yvertice = half2->twin->controlpoint->y;
                    double p0x = half2->vertex->x;
                    double p0y = half2->vertex->y;

                    double v1x = p1xexis - p0x;
                    double v1y = p1yexis - p0y;

                    double v2x = p3xvertice - p0x;
                    double v2y = p3yvertice - p0y;
                    double dot = (v1x * v2x) + (v1y * v2y);
                    double det = v1x * v2y - v1y * v2x;
                    double normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomaexistente = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomaexistente = 360 - angulocomaexistente;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }

                    cout << "Angulo com a EXISTENTE: " << angulocomaexistente << endl;

                    v1x = p1xnova - p0x;
                    v1y = p1ynova - p0y;
                    dot = (v1x * v2x) + (v1y * v2y);
                    det = v1x * v2y - v1y * v2x;
                    normas = sqrt(v1x * v1x + v1y * v1y) * sqrt(v2x * v2x + v2y * v2y);
                    double angulocomanova = acos(dot / normas) * 180 / M_PI;

                    if (det > 0) {
                        angulocomanova = 360 - angulocomanova;
                        cout << "Counter Clock-wise" << endl;
                    } else {
                        cout << "Clock-wise" << endl;
                    }
                    cout << "Angulo com a NOVA: " << angulocomanova << endl;


                    if (angulocomanova < angulocomaexistente) {

                        vertex = new Vertex;
                        controlPoint = new ControlPoint;
                        // Link the from-side of the edge.
                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;
                        he1->prev = half1->prev;
                        he1->next = half2;
                        half2->prev = he1;
                        half1->prev->next = he1;

                        //Buscar--next da he1
                        //he1->next = half->next;

                        // Link the to-side of the edge.
                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;
                        he3->next = half1;

                        half1->prev = he3;

                        he3->prev = half1;

                        //Buscar--prev da he3

                        he1->twin = he3;
                        he3->twin = he1;


                    } else {
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        //Vértice no qual a Half-Edge está chegando
                        vertex->x = listadebezier[i][6];
                        vertex->y = listadebezier[i][7];
                        he1->vertex = vertex;
                        controlPoint->x = listadebezier[i][2];
                        controlPoint->y = listadebezier[i][3];
                        he1->controlpoint = controlPoint;

                        he1->next = half1->twin;
                        half1->twin->prev = he1;
                        he1->prev = half2->next->twin;
                        half2->next->twin->next = he1;


                        //Vértice no qual a Half-Edge está chegando (OPOSTA)
                        vertex = new Vertex;
                        controlPoint = new ControlPoint;

                        vertex->x = listadebezier[i][0];
                        vertex->y = listadebezier[i][1];
                        he3->vertex = vertex;
                        controlPoint->x = listadebezier[i][4];
                        controlPoint->y = listadebezier[i][5];
                        he3->controlpoint = controlPoint;

                        he3->prev = half1->next->twin;
                        he3->next = half2->twin;
                        half2->twin->prev = he3;
                        half1->next->twin->next = he3;

                        he1->twin = he3;
                        he3->twin = he1;

                    }
                }
            }
            break;
        case 2:
            cout << "Caso: 2" << endl;

            if (half2->next != nullptr){
                double p1xexis = half2->twin->controlpoint->x;
                double p1yexis = half2->twin->controlpoint->y;
                double p1xnova = listadebezier[i][2];
                double p1ynova = listadebezier[i][3];

                double p3xvertice = half2->next->controlpoint->x;
                double p3yvertice = half2->next->controlpoint->y;
                double p0x = half2->vertex->x;
                double p0y = half2->vertex->y;

                double v1x = p1xexis - p0x;
                double v1y = p1yexis - p0y;

                double v2x = p3xvertice - p0x;
                double v2y = p3yvertice - p0y;
                double dot = (v1x*v2x) + (v1y*v2y);
                double det = v1x*v2y - v1y*v2x;
                //double norma = sqrt(pow(v1x,2) + pow(v1y,2)) * sqrt(pow(v2x,2) + pow(v2y,2));
                double normas = sqrt(v1x*v1x + v1y*v1y) * sqrt(v2x*v2x + v2y*v2y);
                double divisao = dot/normas;
                double angulocomaexistente =  acos(dot/normas)*180/M_PI;
                if (angulocomaexistente != angulocomaexistente){
                    angulocomaexistente = 0;
                }

                if(v1x*v2y - v1y*v2x < 0) {
                    angulocomaexistente = 360 - angulocomaexistente;
                }

                cout << "Angulo com a EXISTENTE: " << angulocomaexistente << endl;

                v2x = p1xnova - p0x;
                v2y = p1ynova - p0y;
                dot = (v1x*v2x) + (v1y*v2y);
                det = v1x*v2y - v1y*v2x;
                normas = sqrt(v1x*v1x + v1y*v1y) * sqrt(v2x*v2x + v2y*v2y);

                double angulocomanova =  acos(dot/normas)*180/M_PI;

                if(v1x*v2y - v1y*v2x < 0) {
                    angulocomanova = 360 - angulocomanova;
                }


                cout << "Angulo com a NOVA: " << angulocomanova << endl;

                ///SE o angulo da nova for maior que o da antiga no sentido horário, então a linha nova está para dentro do desenho
                if (angulocomanova > angulocomaexistente){

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;
                    // Link the from-side of the edge.
                    //Vértice no qual a Half-Edge está chegando
                    vertex->x = listadebezier[i][6];
                    vertex->y = listadebezier[i][7];
                    he1->vertex = vertex;
                    controlPoint->x = listadebezier[i][2];
                    controlPoint->y = listadebezier[i][3];
                    he1->controlpoint = controlPoint;

                    he1->prev = half2->next->twin;
                    half2->next->twin->next = he1;

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    vertex->x = listadebezier[i][0];
                    vertex->y = listadebezier[i][1];
                    he3->vertex = vertex;
                    controlPoint->x = listadebezier[i][4];
                    controlPoint->y = listadebezier[i][5];
                    he3->controlpoint = controlPoint;

                    he3->next = half2->twin;
                    half2->twin->prev = he3;


                    he1->twin = he3;
                    he3->twin = he1;

                    he1->next = he3;
                    he3->prev = he1;

                } else {

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;
                    // Link the from-side of the edge.
                    //Vértice no qual a Half-Edge está chegando
                    vertex->x = listadebezier[i][6];
                    vertex->y = listadebezier[i][7];
                    he1->vertex = vertex;
                    controlPoint->x = listadebezier[i][2];
                    controlPoint->y = listadebezier[i][3];
                    he1->controlpoint = controlPoint;

                    he1->prev = half2;
                    half2->next = he1;

                    vertex = new Vertex;
                    controlPoint = new ControlPoint;

                    vertex->x = listadebezier[i][0];
                    vertex->y = listadebezier[i][1];
                    he3->vertex = vertex;
                    controlPoint->x = listadebezier[i][4];
                    controlPoint->y = listadebezier[i][5];
                    he3->controlpoint = controlPoint;

                    he3->next = half2->twin->prev->twin;
                    half2->twin->prev->twin->prev = he3;

                    he1->twin = he3;
                    he3->twin = he1;

                    he1->next = nullptr;
                    he3->prev = nullptr;

                    he0 = he1;
                    he2 = he3;

                }
            } else {
                cout << "Ligando: " << listadebezier[i][0] << "," << listadebezier[i][1] << " a "
                     << half2->vertex->x << "," << half2->vertex->y << endl;

                vertex = new Vertex;
                controlPoint = new ControlPoint;

                //Vértice no qual a Half-Edge está chegando
                vertex->x = listadebezier[i][6];
                vertex->y = listadebezier[i][7];
                he1->vertex = vertex;
                controlPoint->x = listadebezier[i][2];
                controlPoint->y = listadebezier[i][3];
                he1->controlpoint = controlPoint;
                he1->next = nullptr;

                //Vértice no qual a Half-Edge está chegando (OPOSTA)
                vertex = new Vertex;
                controlPoint = new ControlPoint;

                vertex->x = listadebezier[i][0];
                vertex->y = listadebezier[i][1];
                he3->vertex = vertex;
                controlPoint->x = listadebezier[i][4];
                controlPoint->y = listadebezier[i][5];
                he3->controlpoint = controlPoint;
                he3->prev = nullptr;

                he0->next = he1;
                he1->prev = he0;

                he2->prev = he3;
                he3->next = he2;

                he0->twin = he2;
                he2->twin = he0;

                he1->twin = he3;
                he3->twin = he1;

                he0 = he1;
                he2 = he3;
            }

            break;
        case 3:
            cout << "Caso: 3" << endl;
            break;
        case 4:
            cout << "Caso: 4" << endl;
            cout << "Novo Ciclo" << endl;

            firstHE = new HalfEdge;
            firstHEo = new HalfEdge;

            vertex = new Vertex;
            controlPoint = new ControlPoint;

            //Vértice que a Half-Edge está chegando
            vertex->x = listadebezier[i][6];
            vertex->y = listadebezier[i][7];

            controlPoint->x = listadebezier[i][2];
            controlPoint->y = listadebezier[i][3];

            firstHE->vertex = vertex;
            firstHE->controlpoint = controlPoint;

            firstHE->next = nullptr;
            firstHE->prev = nullptr;

            vertex = new Vertex;
            controlPoint = new ControlPoint;
            //Vértice que a Half-Edge está chegando (OPOSTA)

            vertex->x = listadebezier[i][0];
            vertex->y = listadebezier[i][1];

            controlPoint->x = listadebezier[i][4];
            controlPoint->y = listadebezier[i][5];

            firstHEo->vertex = vertex;
            firstHEo->controlpoint = controlPoint;

            firstHE->twin = firstHEo;
            firstHEo->twin = firstHE;

            firstHEo->next = nullptr;
            firstHEo->prev = nullptr;

            he0 = firstHE;
            he2 = firstHEo;

            halfedgeslist.push_back(firstHE);

            break;

        }
    }
}

QVector<HalfEdge*> HalfEdgeGraph::separaciclos(){

    cout << "Qtde de HalfEdges somente 1 sentido: " << halfedgeslist.size() << endl;

    int qtde = halfedgeslist.size();

    for (int i = 0; i < qtde; ++i) {

        halfedgeslist.push_back(halfedgeslist[i]->twin);
    }
    cout << "Qtde de HalfEdges com Pares: " << halfedgeslist.size() << endl;

    int j = 0;

    while (halfedgeslist.size() != 0) {

        listadeciclos.push_back(halfedgeslist[0]);
        double area = 0;

        HalfEdge* begin = halfedgeslist[j];
        HalfEdge* half = begin;

        do
        {

            if (half->next == nullptr){
                half->next = half->twin;
            }

            area += half->vertex->x * half->next->vertex->y - half->next->vertex->x * half->vertex->y;

            halfedgeslist.erase(find(halfedgeslist.begin(), halfedgeslist.end(), half));
            cout << "Qtde de ciclos: " << listadeciclos.size() << "-- Qtde de HalfEdges na Lista: " << halfedgeslist.size() << endl;
            cout << half->vertex->x << endl;

            half = half->next;
        }
        while(half != begin);

        cout << area << endl;
        areas.push_back(area);
        cycle.push_back(0);

    }
    return listadeciclos;
}



// 0 - not classified/ 1 - outer /2 - border/ 3- island / 4 -other (CYCLE LIST)

void HalfEdgeGraph::classificaciclos(){

    double areadooutersketchcycle = 0;

    

    //-----ENCONTRA OUTER SKETCH CYCLE---------

    for (int i = 0; i < listadeciclos.size(); ++i) {

        if (areas[i] > areadooutersketchcycle){ //area >0 (CW Cycle e maior que as outras é Outer-Sketch Cycle
            outersketchcycle = listadeciclos[i];
            areadooutersketchcycle = areas[i];
            outersketchcycle->cycle = 1;
        }
    }

    defineciclo(outersketchcycle);
    
    
    //    listadeciclos.erase(find(listadeciclos.begin(), listadeciclos.end(), outersketchcycle));
    //    areas.erase(find(areas.begin(), areas.end(), areadooutersketchcycle));



    //---------------------------------------
    //SEPARA BORDER-CYCLES CICLOS

    HalfEdge* begin = outersketchcycle;
    HalfEdge* outercyclehalf = begin;

    do {

        if (outercyclehalf->twin->cycle == 0){
            for (int i = 0; i < listadeciclos.size(); ++i) {
                if (listadeciclos[i]->cycle == 0){
                    HalfEdge* inicio = listadeciclos[i];
                    HalfEdge* half = inicio;
                    do{
                        if (half == outercyclehalf->twin){
                            half->cycle = 2;
                            defineciclo(half);
                            i = listadeciclos.size();
                            break;
                        }
                        half = half->next;
                    }while (half != inicio);
                }
            }
        }

        outercyclehalf = outercyclehalf->next;

    } while (outercyclehalf != begin);
    //------------------------------------
    //SEPARA ISLAND CYCLES

    for (int i = 0; i < listadeciclos.size(); ++i) {
        if (listadeciclos[i]->cycle == 0) {

            HalfEdge* begin = listadeciclos[i];
            HalfEdge* half = begin;
            double area = 0;
            do {

                area += half->twin->vertex->x * half->controlpoint->y - half->controlpoint->x * half->twin->vertex->y;
                area += half->controlpoint->x * half->twin->controlpoint->y - half->twin->controlpoint->x * half->controlpoint->y;
                area += half->twin->controlpoint->x * half->vertex->y - half->vertex->x * half->twin->controlpoint->y;

                half = half->next;
            } while (begin != half);

            if (area < 0) { //Island Cycle

                listadeciclos[i]->cycle = 3;
                defineciclo(listadeciclos[i]);

            } else if(area > 0 || area == 0){ //Other

                listadeciclos[i]->cycle = 4;
                defineciclo(listadeciclos[i]);

            }
        }
    }
}


// Marca os ciclos com inteiros - 
// 0 - not classified/ 1 - outer /2 - border/ 3- island / 4 -other (CYCLE LIST)
void HalfEdgeGraph::defineciclo(HalfEdge* half){
    HalfEdge* begin = half;

    int ciclo = half->cycle;
    do {
        half->cycle = ciclo;
        
        half = half->next;
    } while (half != begin);
    
}

QVector<HalfEdge*> HalfEdgeGraph::completacontornos (){


    //Pode ser que a primeira half-edge dos ciclos esteja no meio de 2 linhas sugestivas
    //Por isto calcular a distancia entre elas, ajuda a selecionar os contornos mais proximos

    double distancia1 = 0; //distancia entre primeira half-edge e segunda
    double distancia2 = 0; // distancia entre segunda e terceira

    for (int i = 0; i < listadeciclos.size(); ++i) {
        if(listadeciclos[i]->cycle == 2){
            HalfEdge* half = listadeciclos[i];
            HalfEdge* begin = half;

            //
            if (listadeciclos[i]->next == listadeciclos[i]->twin){
                half = half->next->next;
                begin = half;
                listadeciclos[i] = begin;
            }
            do {
                if (half->next == half->twin){ //Linha Sugestiva
                    linhassugestivas.push_back(half);


                    if (linhassugestivas.size() == 3){
                        distancia1 = pow((linhassugestivas[1]->twin->vertex->x - linhassugestivas[0]->twin->vertex->x),2)
                                + pow((linhassugestivas[1]->twin->vertex->y - linhassugestivas[0]->twin->vertex->y),2);
                        distancia1 = sqrt(distancia1);

                        distancia2 = pow((linhassugestivas[2]->twin->vertex->x - linhassugestivas[1]->twin->vertex->x),2)
                                + pow((linhassugestivas[2]->twin->vertex->y - linhassugestivas[1]->twin->vertex->y),2);
                        distancia2 = sqrt(distancia2);

                        if (distancia1 < distancia2){
                            half = half->next;
                            fechahalfedges(linhassugestivas[0], linhassugestivas[1]);

                            linhassugestivas.removeAt(0);
                            linhassugestivas.removeAt(0);
                        } else {
                            half = half->next;

                            fechahalfedges(linhassugestivas[1], linhassugestivas[2]);

                            linhassugestivas.removeAt(1);
                            linhassugestivas.removeAt(1);


                        }
                    }

                }

                half = half->next;
            } while (half != begin);

            if (linhassugestivas.size() > 1){
                fechahalfedges(linhassugestivas[0], linhassugestivas[1]);
                linhassugestivas.removeAt(0);
                linhassugestivas.removeAt(0);
            }




        }
    }


    return listadeciclos;


}


void HalfEdgeGraph::fechahalfedges(HalfEdge *linha1, HalfEdge *linha2)
{


    //----- Extremidade Aberta

    HalfEdge *he0 = new HalfEdge;
    HalfEdge *he2 = new HalfEdge;

    Vertex *vertex = new Vertex;
    ControlPoint *controlPoint = new ControlPoint;

    //Vértice no qual a Half-Edge está chegando
    vertex->x = linha2->vertex->x;
    vertex->y = linha2->vertex->y;
    he0->vertex = vertex;

    controlPoint->x = linha1->vertex->x -(linha1->twin->controlpoint->x - linha1->vertex->x);
    controlPoint->y = linha1->vertex->y -(linha1->twin->controlpoint->y - linha1->vertex->y);

    he0->controlpoint = controlPoint;

    //Vértice no qual a Half-Edge está chegando (OPOSTA)
    vertex = new Vertex;
    controlPoint = new ControlPoint;

    vertex->x = linha1->vertex->x;
    vertex->y = linha1->vertex->y;
    he2->vertex = vertex;

    controlPoint->x = linha2->vertex->x - (linha2->twin->controlpoint->x - linha2->vertex->x);
    controlPoint->y = linha2->vertex->y - (linha2->twin->controlpoint->y - linha2->vertex->y);
    he2->controlpoint = controlPoint;

    he0->twin = he2;
    he2->twin = he0;

    he0->prev = linha1;
    linha1->next = he0;

    he2->next = linha1->twin;
    linha1->twin->prev = he2;

    he0->next = linha2->twin;
    linha2->twin->prev = he0;

    he2->prev = linha2;
    linha2->next = he2;

    he2->cycle = 5;
    defineciclo(he2);


    //--- Extremidade Fechada

    HalfEdge *he1 = new HalfEdge;
    HalfEdge *he3 = new HalfEdge;

    vertex = new Vertex;
    controlPoint = new ControlPoint;

    //Vértice no qual a Half-Edge está chegando
    vertex->x = linha1->twin->vertex->x;
    vertex->y = linha1->twin->vertex->y;
    he1->vertex = vertex;

    controlPoint->x = linha2->twin->vertex->x -(linha2->twin->next->controlpoint->x - linha2->twin->vertex->x);
    controlPoint->y = linha2->twin->vertex->y -(linha2->twin->next->controlpoint->y - linha2->twin->vertex->y);

    he1->controlpoint = controlPoint;

    //Vértice no qual a Half-Edge está chegando (OPOSTA)
    vertex = new Vertex;
    controlPoint = new ControlPoint;

    vertex->x = linha2->twin->vertex->x;
    vertex->y = linha2->twin->vertex->y;
    he3->vertex = vertex;

    controlPoint->x = linha1->twin->vertex->x -(linha1->prev->twin->controlpoint->x - linha1->twin->vertex->x);
    controlPoint->y = linha1->twin->vertex->y -(linha1->prev->twin->controlpoint->y - linha1->twin->vertex->y);
    he3->controlpoint = controlPoint;

    he1->cycle = 5; //5 para diferenciar se é uma half-edge que fecha o contorno aberto ou se é half-edge da borda externa que é 1
    he3->cycle = 2;

    he1->twin = he3;
    he3->twin = he1;

    //Não pode ligar he1 porque ela confunde o ciclo externo

    linha1->prev->next = he3;
    he3->prev = linha1->prev;

    he3->next = linha2->twin->next;
    linha2->twin->next->prev = he3;


    // 0 - not classified/ 1 - outer /2 - border/ 3- island / 4 -other / 5 - Foreground parts / 6 - background parts
    //(CYCLE LIST)
    //Separa membros adjacentes do sketch... Duplica arestas do membro adjacente


    he3->cycle = 2;
    defineciclo(he3);

    //listadeciclos.push_back(he0);
    listadeciclos.push_back(he2);

    linhasmembrosadjacentes.push_back(linha1);
    linhasmembrosadjacentes.push_back(linha2);

}

void HalfEdgeGraph::identificapartessimetricas(){

    for (int i = 0; i < linhasmembrosadjacentes.size(); i=i+2) {
        //Calcular centro das linhas adj e do contorno fechado (p0 e p1);
        double xMaberto = (linhasmembrosadjacentes[i]->vertex->x + linhasmembrosadjacentes[i+1]->vertex->x)/2;
        double yMaberto = (linhasmembrosadjacentes[i]->vertex->y + linhasmembrosadjacentes[i+1]->vertex->y)/2;

        double xMfechado = (linhasmembrosadjacentes[i]->twin->vertex->x + linhasmembrosadjacentes[i+1]->twin->vertex->x)/2;
        double yMfechado = (linhasmembrosadjacentes[i]->twin->vertex->y + linhasmembrosadjacentes[i+1]->twin->vertex->y)/2;

        cout << "Ponto Medio Entre: " << linhasmembrosadjacentes[i]->vertex->x << "," << linhasmembrosadjacentes[i]->vertex->y <<" e "
             << linhasmembrosadjacentes[i+1]->vertex->x <<"," <<linhasmembrosadjacentes[i+1]->vertex->y << " = " << xMaberto << ","<< yMaberto << endl;

        double v0x = xMfechado - xMaberto;
        double v0y = yMfechado - yMaberto;

        //Começa da linha da direita da perna(linha2) vou percorrer em sentido anti-horário
        HalfEdge* half =linhasmembrosadjacentes[i+1]->twin->next ;


        do {

            if (half->twin->cycle == 2){
                double xMhalf = (half->vertex->x + half->twin->vertex->x) /2;
                double yMhalf = (half->vertex->y + half->twin->vertex->y) /2;
                double v1x = xMhalf - xMaberto;
                double v1y = yMhalf - yMaberto;
                double dot = (v0x * v1x) + (v0y * v1y);
                double normas = sqrt(v0x * v0x + v0y * v0y) * sqrt(v1x * v1x + v1y * v1y);
                double anguloalfa = acos(dot / normas) * 180 / M_PI;
                cout << anguloalfa << endl;
                if (anguloalfa < 90){
                    half->twin->cycle = 6;
                    defineciclo(half->twin);

                }

            }


            half = half->next;
        } while (half->twin->cycle != 5);

        //Começa da linha da direita da perna(linha1) vou percorrer em sentido horário as half-edges internas
        half = linhasmembrosadjacentes[i]->prev;

        do {

            if (half->twin->cycle == 2){
                double xMhalf = (half->vertex->x + half->twin->vertex->x) /2;
                double yMhalf = (half->vertex->y + half->twin->vertex->y) /2;
                double v1x = xMhalf - xMaberto;
                double v1y = yMhalf - yMaberto;
                double dot = (v0x * v1x) + (v0y * v1y);
                double normas = sqrt(v0x * v0x + v0y * v0y) * sqrt(v1x * v1x + v1y * v1y);
                double anguloalfa = acos(dot / normas) * 180 / M_PI;
                cout << anguloalfa << endl;
                if (anguloalfa < 90){
                    half->twin->cycle = 6;
                    defineciclo(half->twin);

                }

            }

            half = half->prev;
        } while (half->twin->cycle != 5);

    }


}






