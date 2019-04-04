#ifndef GROUP3D_H
#define GROUP3D_H

#include "transformational.h"
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

class Group3D : public Transformational
{
public:
    Group3D();
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

    void addObject(Transformational *obj);

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    // чтоб группа могла содержать и другие группы, а не только объекты
    QVector<Transformational *> m_objects;
};

#endif // GROUP3D_H
