#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

struct VertexData //структура заполняющяя initCube
{
    VertexData()
    {}
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {}
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void updateCamPos();

    QOpenGLTexture *m_texture;

    double camAngle, camNearPlane, camFarPlane, camSmooth;
    double camPosX, camPosY, camPosZ;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void initShaders();
    void initCube(float width);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;

    QOpenGLBuffer m_arrayBuffer;
    QOpenGLBuffer m_indexBuffer;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;
    QVector3D m_position;
};

#endif // WIDGET_H
