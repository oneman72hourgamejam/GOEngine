#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget (parent)
{
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Widget::resizeGL(int w, int h)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, w / (float)h, 0.1f, 10.0f);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


