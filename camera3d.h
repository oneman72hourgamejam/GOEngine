#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "transformational.h"
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

class Camera3D : public Transformational
{
public:
    Camera3D();
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);
    // значение 0 у функций, чтоб не передавать значение заного в paintGL для камеры
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0);

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
};

#endif // CAMERA3D_H
