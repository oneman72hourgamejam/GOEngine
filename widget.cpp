#include "widget.h"
#include <simpleobject3d.h>
#include <QMouseEvent>
#include <QOpenGLContext>
#include "group3d.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget (parent)
{
    this->camAngle = 45.0;
    this->camNearPlane = 0.01f;
    this->camFarPlane = 100.0f;
    this->camSmooth = 2.0;
    this->camPosX = 0.1;
    this->camPosY = 0.1;
    this->camPosZ = 5.0;
    this->specularFactor = 60.0;
    this->ambientFactor = 0.1;
    this->lightPower = 5.0f;
    this->lightPosX = 0.0;
    this->lightPosY = 0.0;
    this->lightPosZ = 0.0;
    this->lightPosW = 1.0;
    this->lightColorR = 1.0;
    this->lightColorG = 1.0;
    this->lightColorB = 1.0;
    this->lightColorA = 1.0;
    m_position = QVector3D(camPosX, camPosY, -camPosZ);
    m_lightPower = lightPower;
    m_specularFactor = specularFactor;
    m_ambientFactor = ambientFactor;
    m_lightColor = QVector4D(lightColorR, lightColorG, lightColorB, lightColorA);
    m_lightPosition = QVector4D(lightPosX, lightPosY, lightPosZ, lightPosW);
}

Widget::~Widget()
{

}

void Widget::updateCamPos()
{
    m_position = QVector3D(camPosX, camPosY, -camPosZ);
    m_lightPower = lightPower;
    m_specularFactor = specularFactor;
    m_ambientFactor = ambientFactor;
    m_lightColor = QVector4D(lightColorR, lightColorG, lightColorB, lightColorA);
    m_lightPosition = QVector4D(lightPosX, lightPosY, lightPosZ, lightPosW);

    update();
}

void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    float step = 1.0f;
    m_groups.append(new Group3D);

    for (int x = -step; x <= step; x += step)
    {
        for (int y = -step; y <= step; y += step)
        {
            for (int z = -step; z <= step; z += step)
            {
                initCube(0.5f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[0]->translate(QVector3D(-2.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);

    for (int x = -step; x <= step; x += step)
    {
        for (int y = -step; y <= step; y += step)
        {
            for (int z = -step; z <= step; z += step)
            {
                initCube(0.5f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[1]->translate(QVector3D(2.0f, 0.0f, 0.0f));

    // содержит в себе обе группы объектов
    m_groups.append(new Group3D);
    m_groups[2]->addObject(m_groups[0]);
    m_groups[2]->addObject(m_groups[1]);

    m_TransformObjects.append(m_groups[2]);
}

void Widget::resizeGL(int w, int h)
{
    m_projectionMatrix.setToIdentity();
    // плоскость отсечения
    m_projectionMatrix.perspective(camAngle, w / (float)h, camNearPlane, camFarPlane);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(m_position);
    viewMatrix.rotate(m_rotation);

    // биндим программу, чтоб иметь к ней доступ
    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);
    // QVector4D(0.0, 0.0, 0.0, 1.0) - вершина, а не вектор
    m_program.setUniformValue("u_lightColor", m_lightColor);
    m_program.setUniformValue("u_lightPosition", m_lightPosition);
    m_program.setUniformValue("u_lightPower", m_lightPower);
    m_program.setUniformValue("u_specularFactor", m_specularFactor);
    m_program.setUniformValue("u_ambientFactor", m_ambientFactor);

    for(int i = 0; i < m_TransformObjects.size(); ++i)
    {
        m_TransformObjects[i]->draw(&m_program, context()->functions());
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        // локальные координаты мыши относительно левого верхнего угла окна
        m_mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    // текущяя позиция - предыдущяя
    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    // сохраняем текущюю позицию
    m_mousePosition = QVector2D(event->localPos());

    // плавный угол поворота
    float angle = diff.length() / camSmooth;

    // вектор осуществляющий поворот (перпендикулярный вектор)
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);
    // вектор вокруг которого будет осуществляться поворот и угол поворота,
    // домножаем на м_ротейшн чтоб продолжить поворот с текущей позиции
    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    // дельта - на сколько повернули колесико
    if(event->delta() > 0)
    {
        camPosZ += 0.25f;
    }
    else if(event->delta() < 0)
    {
        camPosZ -= 0.25f;
    }

    m_position = QVector3D(camPosX, camPosY, -camPosZ);
    update();
}

void Widget::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
    {
        close();
    }
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
    {
        close();
    }

    // объеденяем все шейдеры в один
    if(!m_program.link())
    {
        close();
    }
}

void Widget::initCube(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

    // хватило бы и short
    QVector<GLuint> indexes;
    for(int i = 0; i < 24; i += 4)
    {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    //m_image = new QOpenGLTexture(QImage(":/cube.png");
    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage(":/cube.png")));
}


