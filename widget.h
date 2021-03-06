#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QBasicTimer>

class SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;

class Widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void updateCamPos();

//    QImage *m_image;

    double camAngle, camNearPlane, camFarPlane, camSmooth;
    //double camPosX, camPosY, camPosZ;
    double specularFactor, ambientFactor, lightPower;
    double lightPosX, lightPosY, lightPosZ, lightPosW;
    double lightColorR, lightColorG, lightColorB, lightColorA;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initShaders();
    void initCube(float width);
    // константа по ссылке - строка не будет копироваться, будет передаваться адресс
    void loadObj(const QString &path);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkybox;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;
    //QVector3D m_position;
    QVector4D m_lightPosition;
    QVector4D m_lightColor;
    float m_lightPower;
    float m_ambientFactor, m_specularFactor;

    QVector<SimpleObject3D *> m_objects;
    QVector<Transformational *> m_TransformObjects;
    QVector<Group3D *> m_groups;

    QBasicTimer m_timer;

    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;

    Camera3D *m_camera;
    SkyBox *m_skybox;
};

#endif // WIDGET_H
