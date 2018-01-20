#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

# include <QtOpenGL>
# include <QOpenGLWidget>
# include <QOpenGLFunctions>
# include <QOpenGLBuffer>
//# include <gl/GLU.h>
//# include <gl/GL.h>

class OpenGLWidget : public QOpenGLWidget ,protected
        QOpenGLFunctions
{
    Q_OBJECT


public :
    explicit OpenGLWidget ( QWidget *parent = 0 );
    ~ OpenGLWidget();
    void paintGL ();
    void resizeGL (int w, int h);
    void initializeGL ();
public slots :
    void toggleBackgroundColor ( bool changeBColor );

signals :
};

#endif // OPENGLWIDGET_H
