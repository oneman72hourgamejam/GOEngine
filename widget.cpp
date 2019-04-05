#include "widget.h"
#include <simpleobject3d.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtMath>
#include <QOpenGLContext>
#include "camera3d.h"
#include "group3d.h"
#include "skybox.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget (parent)
{
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0, 0.0, -5.0));
    this->camAngle = 45.0;
    this->camNearPlane = 0.01f;
    this->camFarPlane = 1000.0f;
    this->camSmooth = 2.0;
    //this->camPosX = 0.1;
    //this->camPosY = 0.1;
    //this->camPosZ = 5.0;
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
    //m_position = QVector3D(camPosX, camPosY, -camPosZ);
    m_lightPower = lightPower;
    m_specularFactor = specularFactor;
    m_ambientFactor = ambientFactor;
    m_lightColor = QVector4D(lightColorR, lightColorG, lightColorB, lightColorA);
    m_lightPosition = QVector4D(lightPosX, lightPosY, lightPosZ, lightPosW);
}

Widget::~Widget()
{
    delete m_camera;
    for(int i = 0; i < m_objects.size(); ++i)
    {
        delete m_objects[i];
    }
    for(int i = 0; i < m_groups.size(); ++i)
    {
        delete m_groups[i];
    }
    for(int i = 0; i < m_TransformObjects.size(); ++i)
    {
        // решить проблему с удалением
        //delete m_TransformObjects[i];
    }
}

void Widget::updateCamPos()
{
    //m_position = QVector3D(camPosX, camPosY, -camPosZ);
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

//    float step = 1.0f;
    m_groups.append(new Group3D);

    initCube(2.0f);
    m_objects[0]->translate(QVector3D(0.0, 0.0, 0.0));
    m_groups[0]->addObject(m_objects[0]);

//    for (int x = -step; x <= step; x += step)
//    {
//        for (int y = -step; y <= step; y += step)
//        {
//            for (int z = -step; z <= step; z += step)
//            {
//                initCube(0.5f);
//                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
//                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
//            }
//        }
//    }
//    m_groups[0]->translate(QVector3D(-2.0f, 0.0f, 0.0f));

//    m_groups.append(new Group3D);

//    for (int x = -step; x <= step; x += step)
//    {
//        for (int y = -step; y <= step; y += step)
//        {
//            for (int z = -step; z <= step; z += step)
//            {
//                initCube(0.5f);
//                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
//                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
//            }
//        }
//    }
//    m_groups[1]->translate(QVector3D(2.0f, 0.0f, 0.0f));

    // содержит в себе обе группы объектов
    m_groups.append(new Group3D);

    m_groups[1]->addObject(m_groups[0]);

//    m_groups[2]->addObject(m_groups[0]);
//    m_groups[2]->addObject(m_groups[1]);

    m_TransformObjects.append(m_groups[1]);

//    m_TransformObjects.append(m_groups[2]);

    loadObj(":/s.obj");
    m_objects[m_objects.size() - 1]->translate(QVector3D(3.0, 0.0, 0.0));
    m_TransformObjects.append(m_objects[m_objects.size() - 1]);

    m_groups[0]->addObject(m_camera);

    m_skybox = new SkyBox(10, QImage(":/skybox.png"));

    m_timer.start(30, this);
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

//    QMatrix4x4 viewMatrix;
//    viewMatrix.setToIdentity();
//    viewMatrix.translate(m_position);
//    viewMatrix.rotate(m_rotation);
    // вначале отрисовываем скайбокс
    m_programSkybox.bind();
    m_programSkybox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_programSkybox);
    m_skybox->draw(&m_programSkybox, context()->functions());
    m_programSkybox.release();

    // биндим программу, чтоб иметь к ней доступ
    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    //m_program.setUniformValue("u_viewMatrix", viewMatrix);
    // QVector4D(0.0, 0.0, 0.0, 1.0) - вершина, а не вектор
    m_program.setUniformValue("u_lightColor", m_lightColor);
    m_program.setUniformValue("u_lightPosition", m_lightPosition);
    m_program.setUniformValue("u_lightPower", m_lightPower);
    m_program.setUniformValue("u_specularFactor", m_specularFactor);
    m_program.setUniformValue("u_ambientFactor", m_ambientFactor);

    m_camera->draw(&m_program);
    for(int i = 0; i < m_TransformObjects.size(); ++i)
    {
        m_TransformObjects[i]->draw(&m_program, context()->functions());
    }
    // чтоб забиндить новую шейдерную программу - старую нужно освободить
    m_program.release();
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
    //m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    // дельта - на сколько повернули колесико
    if(event->delta() > 0)
    {
        //camPosZ += 0.25f;
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    }
    else if(event->delta() < 0)
    {
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
        //camPosZ -= 0.25f;
    }

    //m_position = QVector3D(camPosX, camPosY, -camPosZ);
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{
//    Q_UNUSED(event);

//    for(int i = 0; i < m_objects.size(); ++i)
//    {
//        if(i % 2 == 0)
//        {
//            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleObject)));
//            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleObject)));
//        }
//        else
//        {
//            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qSin(angleObject)));
//            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleObject)));
//        }
//    }

//    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, qSin(angleGroup1)));
//    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qSin(angleGroup1)));

//    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleGroup1)));
//    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qCos(angleGroup1)));

//    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleMain)));
//    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleMain)));

//    angleObject += M_PI / 180.0f;
//    angleGroup1 += M_PI / 360.0f;
//    angleGroup2 -= M_PI / 360.0f;
//    angleMain += M_PI / 720.0f;

//    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->addObject(m_camera);
        break;
    case Qt::Key_Right:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->addObject(m_camera);
        break;
    case Qt::Key_Down:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->delObject(m_camera);
        break;
    case Qt::Key_Up:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->delObject(m_camera);
        QMatrix4x4 tmp;
        tmp.setToIdentity();
        m_camera->setGlobalTransform(tmp);
        break;
    }

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

    if(!m_programSkybox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh"))
    {
        close();
    }
    if(!m_programSkybox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh"))
    {
        close();
    }
    if(!m_programSkybox.link())
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

void Widget::loadObj(const QString &path)
{
    QFile objFile(path);
    if(!objFile.exists())
    {
        qDebug() << "File not found";
    }

    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    while(!input.atEnd())
    {
        QString str = input.readLine();
        QStringList list = str.split(" ");

        if(list[0] == "#")
        {
            qDebug() << "This is comment:" << str;
            continue;
        }
        else if (list[0] == "mtllib")
        {
            qDebug() << "File with materials: " << str;
            continue;
        }
        else if (list[0] == "v")
        {
            coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(),list[3].toFloat()));
            continue;
        }
        else if (list[0] == "vt")
        {
            texCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));
            continue;
        }
        else if (list[0] == "vn")
        {
            normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(),list[3].toFloat()));
            continue;
        }
        else if (list[0] == "f")
        {
            for(int i = 1; i <= 3; ++i)
            {
                QStringList vert = list[i].split("/");
                vertexes.append(VertexData(coords[vert[0].toLong() - 1], texCoords[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]));
                indexes.append(indexes.size());
            }
            continue;
        }
    }

    objFile.close();

    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage(":/cube.png")));
}


