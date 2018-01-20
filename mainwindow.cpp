#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<stdlib.h>

#include <stdio.h>
#include <string.h>
#include <float.h>
#include <fstream>

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"


#include <QMessageBox>
#include <QPolygon>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setPen(QPen(Qt::black, 7, Qt::SolidLine));

    if (imagemcarregada == true){

        gerarbf(&painter);


    }
}

void MainWindow::showFileOpenDialog()
{

    QString fileName;
    QByteArray fileFormat = "svg";

    fileName = QFileDialog :: getOpenFileName (this ,"Open File",QDir::homePath(), QString("\%1 Files (*.\%2)").arg(QString (fileFormat.toUpper())).arg(QString(fileFormat)));
    //fileName = "/home/saulo/Dropbox/Saulo/ProjetoDoutorado/Apps/gato.svg";
    const char *file = fileName.toStdString().c_str();

        if (!fileName.isEmpty()){
            imagemcarregada = true;
        }

    // Load

    NSVGimage* image;

    //image = nsvgParseFromFile("gato.svg", "px", 96);

    image = nsvgParseFromFile(file, "px", 96);

    ncurvas = 0;
    //QTime t;
    //t.start();

    for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next) {
        for (NSVGpath *path = shape->paths; path != NULL; path = path->next) {
            for (int i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];


                QVector<double> curva(8);
                curva[0] = (double)p[0] ;
                curva[1] = (double)p[1] ;
                curva[2] = (double)p[2] ;
                curva[3] = (double)p[3] ;
                curva[4] = (double)p[4] ;
                curva[5] = (double)p[5] ;
                curva[6] = (double)p[6] ;
                curva[7] = (double)p[7] ;

                if (!((curva[0] == curva [6]) && (curva[1] == curva[7]) && curva[2] == curva [4])){
                    listadecurvas.push_back(curva);
                    //qDebug () << curva;

                    ncurvas += 1;
                }

            }
        }
    }

    emit statusBarMessage ( QString (" Tamanho : \%1 x \%2 -- Número de Curvas Bezier \%3" ).arg(image->width).arg(image->height).arg(ncurvas));

    // --------------------------
    // --------------------------
    // CRIA ESTRUTURA DE HALF-EDGES

    qDebug () << ncurvas;
    HalfEdgeGraph g;

    g.geragrafodesemiarestas(listadecurvas);

    listadeciclos = g.separaciclos();

    g.classificaciclos();

    listadeciclos = g.completacontornos();

    cout << listadeciclos.size() << endl;

    g.identificapartessimetricas();
    imagemcarregada = true;
    this->update();


    // qDebug ("Tempo de Execução: %d milissegundos", t.elapsed());

}

void MainWindow::gerarbf(QPainter *painter){

    QVector <QPainterPath> listadepaths;
    double proporcaodereducao = 1;

    for (int i = 0 ; i < listadeciclos.size() ; i++){

        HalfEdge* begin = listadeciclos[i];
        HalfEdge* half = begin;
        QPainterPath totalPath;
        do
        {
            QPainterPath myPath;

            myPath.moveTo(half->twin->vertex->x*proporcaodereducao,half->twin->vertex->y*proporcaodereducao);
            myPath.cubicTo(half->controlpoint->x*proporcaodereducao,half->controlpoint->y*proporcaodereducao,
                           half->twin->controlpoint->x*proporcaodereducao,half->twin->controlpoint->y*proporcaodereducao,
                           half->vertex->x*proporcaodereducao,half->vertex->y*proporcaodereducao);


            totalPath.addPath(myPath);
            if (half->cycle != half->next->cycle){
                break;
            }
            half = half->next;
        }
        while(half != begin);

        listadepaths.push_back(totalPath);
    }

    for (int i = 0 ; i < listadeciclos.size() ; i++){

        std::vector<QVector3D>knownPoints;
        std::vector<QVector3D>knownNormals;

        std::vector<QVector3D>totalPoints;
        std::vector<QVector3D>totalNormals;


        switch (listadeciclos[i]->cycle) {

        case 1:
            painter->setPen(QPen(Qt::black, 7, Qt::SolidLine));
            //painter->drawPath(listadepaths[i]);
            break;
        case 2:
            painter->setPen(QPen(QColor(51, 153, 255), 4, Qt::SolidLine));
            painter->drawPath(listadepaths[i]);
            break;
        case 3:
            painter->setPen(QPen(QColor(51, 204, 51), 7, Qt::SolidLine));
            painter->drawPath(listadepaths[i]);
            break;
        case 4:
            painter->setPen(QPen(Qt::red, 3, Qt::SolidLine));
            //painter->drawPath(listadepaths[i]);
            break;
        case 5:
            painter->setPen(QPen(Qt::red, 3, Qt::DotLine));
            painter->drawPath(listadepaths[i]);
            break;
        case 6:
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
            //painter->drawPath(listadepaths[i]);

        }

        if (listadeciclos[i]->cycle == 2){

            for (double var = listadepaths[i].length(); var > 0 ; var = var - 10) {

                double percent = var / listadepaths[i].length();

                QLineF angleline;
                /* Set the origin: */
                angleline.setP1(listadepaths[i].pointAtPercent(percent));
                QVector3D ponto;

                ponto.setX(listadepaths[i].pointAtPercent(percent).x());
                ponto.setY(listadepaths[i].pointAtPercent(percent).y());
                ponto.setZ(0);
                knownPoints.push_back(ponto);

                /* Set the angle and length: */
                angleline.setAngle(listadepaths[i].angleAtPercent(percent)-90);
                angleline.setLength(1);
                QVector3D vetornormal(0,0,0);

                vetornormal.setX(angleline.x2()-angleline.x1());
                vetornormal.setY(angleline.y2()-angleline.y1());

                knownNormals.push_back(vetornormal);

                painter->drawLine(ponto.x(), ponto.y(),ponto.x() + vetornormal.x()*1,ponto.y() + vetornormal.y()*1);
            }

            //std::string outFile = "rbf.data";

            std::string outFile = "rbf";
            //std::string s = ;
            outFile.append("_ID");
            outFile.append(std::to_string(listadeciclos[i]->cycle));
            outFile.append("_Ciclo");
            outFile.append(std::to_string(i));
            outFile.append(".data");

            QFont font = painter->font() ;
            font.setPointSize ( 18 );
            //font.setWeight(QFont::DemiBold);
            painter->setFont(font);

            QString s = QString::number(i);

            QRectF boundingbox = listadepaths[i].boundingRect();

            qDebug ()<< "Bounding box size: " <<  boundingbox.size() << " - Ciclo:" << i;

            painter->drawText(boundingbox, Qt::BottomRightCorner, s);

            ///pode ser via seletor de arquivo.
            ///Preencha os vetores com os pontos do desenho e as normais conhecidas
            ///precisamos pontos 3D; preencha o z com 0
            ///
            ///
            ///

            //////////PREENCHER OS VETORES KNOWNPOINTS & KNOWNNORMALS

            //EXPORTA DADOS RBF

            QPolygon poligonodabezier;

            for(QVector3D pi:knownPoints){
                totalPoints.push_back(pi);
                poligonodabezier << QPoint (pi.x(),pi.y());
            }

            for(QVector3D ui:knownNormals){
                ui.normalize();
                totalNormals.push_back(ui);
            }

            double xmin = boundingbox.left();
            double xmax = boundingbox.right();
            double ymin = boundingbox.top();
            double ymax = boundingbox.bottom();

            cout <<  outFile << endl;
            // qDebug () << xmax-xmin;
            // qDebug () << ymax-ymin;

            painter->drawRect(boundingbox);


            //double spacing = (xmax - xmin) /10;  //5.0;///Espacamento do grid
            double spacing = 20;
            int r,g,b;
            // qDebug ()<< "Spacing" << spacing;
            for(double x = xmin; x < xmax; x = x + spacing)
            {
                for(double y = ymin; y < ymax; y = y + spacing)
                {
                    QPoint ponto (x,y);

                    if (poligonodabezier.containsPoint(ponto,Qt::WindingFill)){

                        ///para cada ponto do grid... que esteja dentro do contorno
                        QVector3D p(x,y,0);
                        //calcula o peso
                        double weight = 0;
                        for(QVector3D pi:knownPoints)
                        {
                            weight += 1.0/((p-pi).lengthSquared());///acho que isso retorna a norma ao 2

                            ///PROBLEMA
                            /// Talvez tenha que trazer os pontos para 0~1
                        }

                        ///calcula a normal
                        QVector3D n(0,0,0);
                        for(int i = 0; i <knownNormals.size(); i++)
                        {
                            QVector3D pi = knownPoints.at(i);
                            QVector3D ui = knownNormals.at(i);
                            n += (ui / (p-pi).lengthSquared());///acho que isso retorna a norma ao 2
                        }

                        n = n / weight;

                        n.setZ(sqrt(1 - n.x()*n.x() - n.y()*n.y()));

                        if  (n.z() < .5){

                            r = (abs(n.x()) + 1)/2*255;
                            g = (abs(n.y()) + 1)/2*255;
                            b = (n.z() + 1)/2*255;

                            painter->setPen(QPen(QColor(r,g,b), 1, Qt::SolidLine));
                            painter->drawLine(ponto.x(), ponto.y(), ponto.x(), ponto.y());

                            ///Tem que dar uma altura para esses pontos, eu tô chutando o z da normal, mas nao sei se serve.
                            //p.setZ(n.z());

                            ///calculou para um ponto no grid a norma dele. agora coloca no vetor final.
                            //p.setZ(1);

                            p.setZ(n.z()*(boundingbox.width()/10));
                            totalPoints.push_back(p);
                            totalNormals.push_back(n);
                            //qDebug () << p;
                            //qDebug () << "Z+ .. " << "Ponto: " <<p << "Normal: "<< n;
                            p.setZ(n.z()*(-boundingbox.width()/10));
                            totalPoints.push_back(p);
                            //qDebug () << p;
                            n.setZ(n.z()*-1);
                            //n = n*-1;
                            totalNormals.push_back(n);

                            //qDebug () << "Z- .. " << "Ponto: " <<p << "Normal: "<< n;
                        }
                    }


                }

            }

            ///agora abre um arquivo pra salvar estes dados para serem entrada do RBF. Esse é o formato que eu uso
            std::ofstream fOut;
            fOut.open(outFile.c_str());
            fOut << "3" <<std::endl;
            fOut << knownPoints.size() <<std::endl;
            for(QVector3D p:knownPoints)
            {
                fOut << p.x() <<" " << p.y()<< " " << p.z()<< std::endl;
            }

            fOut << totalNormals.size() << std::endl;

            for(int i = 0; i < totalNormals.size(); i++)
            {

                QVector3D p = totalPoints.at(i);
                QVector3D n = totalNormals.at(i);

                fOut << n.x() <<" " << n.y()<< " " << n.z()<< std::endl;
                fOut << p.x() <<" " << p.y()<< " " << p.z()<< std::endl;
            }
            fOut << "0" << std::endl;
            fOut.close();

        } else if (listadeciclos[i]->cycle == 5 ||listadeciclos[i]->cycle == 3 ) {

            for (int j = 0 ; j < listadeciclos.size() ; j++){

                QPointF pontodomembro(listadeciclos[i]->vertex->x, listadeciclos[i]->vertex->y);

                if (((listadeciclos[j]->cycle == 2 && listadepaths[i].intersects(listadepaths[j]) == true) || listadeciclos[j]->cycle == 2 && (listadepaths[j].boundingRect().contains(pontodomembro)))){

                    qDebug () << i << "COM " << j;

                    //Poligono do Corpo abaixo do membro sugestivo
                    QPolygon poligonodocorpo;
                    QPolygon poligonodomembro;

                    std::vector<QVector3D>PointsdoCorpo;
                    std::vector<QVector3D>NormalsdoCorpo;


                    for (double var = listadepaths[j].length(); var > 0 ; var = var - 10) {

                        double percent = var / listadepaths[j].length();
                        poligonodocorpo << QPoint (listadepaths[j].pointAtPercent(percent).x(),listadepaths[j].pointAtPercent(percent).y());

                        QLineF angleline;
                        /* Set the origin: */
                        angleline.setP1(listadepaths[i].pointAtPercent(percent));
                        QVector3D ponto;

                        ponto.setX(listadepaths[i].pointAtPercent(percent).x());
                        ponto.setY(listadepaths[i].pointAtPercent(percent).y());
                        ponto.setZ(0);
                        PointsdoCorpo.push_back(ponto);

                        /* Set the angle and length: */
                        angleline.setAngle(listadepaths[i].angleAtPercent(percent)-90);
                        angleline.setLength(1);
                        QVector3D vetornormal(0,0,0);

                        vetornormal.setX(angleline.x2()-angleline.x1());
                        vetornormal.setY(angleline.y2()-angleline.y1());

                        NormalsdoCorpo.push_back(vetornormal);
                    }


                    //Poligono do Membro acima do Corpo

                    for (double var = listadepaths[i].length(); var > 0 ; var = var - 10) {
                        double percent = var / listadepaths[i].length();
                        poligonodomembro << QPoint (listadepaths[i].pointAtPercent(percent).x(),listadepaths[i].pointAtPercent(percent).y());

                        QLineF angleline;
                        /* Set the origin: */
                        angleline.setP1(listadepaths[i].pointAtPercent(percent));
                        QVector3D ponto;

                        ponto.setX(listadepaths[i].pointAtPercent(percent).x());
                        ponto.setY(listadepaths[i].pointAtPercent(percent).y());
                        ponto.setZ(0);
                        knownPoints.push_back(ponto);

                        /* Set the angle and length: */
                        angleline.setAngle(listadepaths[i].angleAtPercent(percent)-90);
                        angleline.setLength(1);
                        QVector3D vetornormal(0,0,0);

                        vetornormal.setX(angleline.x2()-angleline.x1());
                        vetornormal.setY(angleline.y2()-angleline.y1());

                        knownNormals.push_back(vetornormal);

                        painter->drawLine(ponto.x(), ponto.y(),ponto.x() + vetornormal.x()*1,ponto.y() + vetornormal.y()*1);
                    }


                    QFont font = painter->font() ;
                    font.setPointSize ( 18 );
                    //font.setWeight(QFont::DemiBold);
                    painter->setFont(font);

                    QString s = QString::number(i);

                    QRectF boundingbox = listadepaths[i].boundingRect();

                    qDebug ()<< "Bounding box size: " <<  boundingbox.size() << " - Ciclo:" << i;

                    painter->drawText(boundingbox, Qt::BottomRightCorner, s);

                    ///pode ser via seletor de arquivo.
                    ///Preencha os vetores com os pontos do desenho e as normais conhecidas
                    ///precisamos pontos 3D; preencha o z com 0
                    ///
                    ///
                    ///

                    //////////PREENCHER OS VETORES KNOWNPOINTS & KNOWNNORMALS

                    //EXPORTA DADOS RBF

                    double xmin = boundingbox.left();
                    double xmax = boundingbox.right();
                    double ymin = boundingbox.top();
                    double ymax = boundingbox.bottom();

                    painter->drawRect(boundingbox);

                    QVector3D pontomaisalto(0,0,0);
                    QVector3D normaldopontomaisalto (0,0,0);



                    //double spacing = (xmax - xmin) /10;  //5.0;///Espacamento do grid
                    double spacing = 20;
                    int r,g,b;
                    // qDebug ()<< "Spacing" << spacing;
                    for(QVector3D pi:knownPoints){
                        totalPoints.push_back(pi);

                    }

                    for(QVector3D ui:knownNormals){
                        ui.normalize();
                        totalNormals.push_back(ui);
                    }

                    ///Calcula Normais
                    for(double x = xmin; x < xmax; x = x + spacing)
                    {
                        for(double y = ymin; y < ymax; y = y + spacing)
                        {
                            QPoint ponto (x,y);

                            if (poligonodomembro.containsPoint(ponto,Qt::WindingFill)){

                                ///para cada ponto do grid... que esteja dentro do contorno
                                QVector3D p(x,y,0);
                                //calcula o peso
                                double weight = 0;
                                for(QVector3D pi:knownPoints)
                                {
                                    weight += 1.0/((p-pi).lengthSquared());///acho que isso retorna a norma ao 2

                                    ///PROBLEMA
                                    /// Talvez tenha que trazer os pontos para 0~1
                                }

                                ///calcula a normal
                                QVector3D n(0,0,0);
                                for(int i = 0; i <knownNormals.size(); i++)
                                {
                                    QVector3D pi = knownPoints.at(i);
                                    QVector3D ui = knownNormals.at(i);
                                    n += (ui / (p-pi).lengthSquared());///acho que isso retorna a norma ao 2
                                }

                                n = n / weight;

                                n.setZ(sqrt(1 - n.x()*n.x() - n.y()*n.y()));

                                if  (n.z() < .5){

                                    r = (abs(n.x()) + 1)/2*255;
                                    g = (abs(n.y()) + 1)/2*255;
                                    b = (n.z() + 1)/2*255;

                                    painter->setPen(QPen(QColor(r,g,b), 1, Qt::SolidLine));
                                    painter->drawLine(ponto.x(), ponto.y(), ponto.x(), ponto.y());

                                    ///Tem que dar uma altura para esses pontos, eu tô chutando o z da normal, mas nao sei se serve.
                                    //p.setZ(n.z());

                                    ///calculou para um ponto no grid a norma dele. agora coloca no vetor final.
                                    //p.setZ(1);

                                    p.setZ(n.z()*(boundingbox.width()/10));
                                    totalPoints.push_back(p);
                                    totalNormals.push_back(n);
                                    p.setZ(n.z()*(-boundingbox.width()/10));
                                    totalPoints.push_back(p);
                                    n.setZ(n.z()*-1);
                                    totalNormals.push_back(n);
                                }

                                if (poligonodocorpo.containsPoint(ponto, Qt::WindingFill) && n.z() > normaldopontomaisalto.z()){
                                    normaldopontomaisalto = n;
                                    pontomaisalto.setX(ponto.x());
                                    pontomaisalto.setY(ponto.y());
                                }
                            }
                        }
                    }



                    //calcula o peso
                    double weight = 0;
                    for(QVector3D pi:PointsdoCorpo)
                    {
                        weight += 1.0/((pontomaisalto-pi).lengthSquared());///acho que isso retorna a norma ao 2

                        ///PROBLEMA
                        /// Talvez tenha que trazer os pontos para 0~1
                    }

                    ///calcula a normal
                    QVector3D n(0,0,0);
                    for(int i = 0; i <NormalsdoCorpo.size(); i++)
                    {
                        QVector3D pi = PointsdoCorpo.at(i);
                        QVector3D ui = NormalsdoCorpo.at(i);
                        n += (ui / (pontomaisalto-pi).lengthSquared());///acho que isso retorna a norma ao 2
                    }

                    n = n / weight;

                    n.setZ(sqrt(1 - n.x()*n.x() - n.y()*n.y()));

                    double alturadomembro = n.z()*(poligonodocorpo.boundingRect().width()/10);

                    painter->setPen(QPen(Qt::black, 7, Qt::SolidLine));
                    painter->drawEllipse(pontomaisalto.x(), pontomaisalto.y(),20,20);
                    qDebug () << pontomaisalto;



                    //std::string outFile = "rbf.data";

                    std::string outFile = "rbf";
                    //std::string s = ;
                    outFile.append("_ID");
                    outFile.append(std::to_string(listadeciclos[i]->cycle));
                    outFile.append("_Ciclo");
                    outFile.append(std::to_string(i));
                    outFile.append("_1");
                    outFile.append(".data");

                    ///agora abre um arquivo pra salvar estes dados para serem entrada do RBF. Esse é o formato que eu uso
                    std::ofstream fOut1;
                    fOut1.open(outFile.c_str());
                    fOut1 << "3" <<std::endl;
                    fOut1 << knownPoints.size() <<std::endl;
                    for(QVector3D p:knownPoints)
                    {
                        fOut1 << p.x() <<" " << p.y()<< " " << p.z()+alturadomembro << std::endl;
                    }

                    fOut1 << totalNormals.size() << std::endl;

                    for(int i = 0; i < totalNormals.size(); i++)
                    {

                        QVector3D p = totalPoints.at(i);
                        QVector3D n = totalNormals.at(i);

                        fOut1 << n.x() <<" " << n.y()<< " " << n.z()<< std::endl;
                        fOut1 << p.x() <<" " << p.y()<< " " << p.z()+alturadomembro<< std::endl;
                    }
                    fOut1 << "0" << std::endl;
                    fOut1.close();

                    outFile = "rbf";
                    //std::string s = ;
                    outFile.append("_ID");
                    outFile.append(std::to_string(listadeciclos[i]->cycle));
                    outFile.append("_Ciclo");
                    outFile.append(std::to_string(i));
                    outFile.append("_2");
                    outFile.append(".data");

                    ///agora abre um arquivo pra salvar estes dados para serem entrada do RBF. Esse é o formato que eu uso
                    std::ofstream fOut;
                    fOut.open(outFile.c_str());
                    fOut << "3" <<std::endl;
                    fOut << knownPoints.size() <<std::endl;
                    for(QVector3D p:knownPoints)
                    {
                        fOut << p.x() <<" " << p.y()<< " " << p.z()-alturadomembro << std::endl;
                    }

                    fOut << totalNormals.size() << std::endl;

                    for(int i = 0; i < totalNormals.size(); i++)
                    {

                        QVector3D p = totalPoints.at(i);
                        QVector3D n = totalNormals.at(i);

                        fOut << n.x() <<" " << n.y()<< " " << n.z()<< std::endl;
                        fOut << p.x() <<" " << p.y()<< " " << p.z()-alturadomembro<< std::endl;
                    }
                    fOut << "0" << std::endl;
                    fOut.close();



                }
            }


        }

    }




}



