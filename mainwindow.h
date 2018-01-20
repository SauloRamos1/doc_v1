#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QPainter>

#include <QFileDialog>

#include <QList>
#include <QVector>
#include <QDebug>

#include <grafo.h>
#include <halfedge.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int ncurvas = 0;
    bool imagemcarregada = false;
    bool desenhargrafo = false;
    float imagewidth;
    float imageheigth;


    QVector<QVector<double>> listadecurvas;
    QVector<HalfEdge*> listadeciclos;   

    QTime t;

    void classificaciclos();
    void gerarbf(QPainter *painter);



private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *e);


private slots:


signals :
    void statusBarMessage ( QString );
public slots :
    void showFileOpenDialog ();




};


#endif // MAINWINDOW_H

