#include "widget.h"

#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget (parent), m_texture(0), m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    this->camAngle = 45.0;
    this->camNearPlane = 0.1;
    this->camFarPlane = 20.0;
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
    initCube(1.0f);
}

void Widget::resizeGL(int w, int h)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(camAngle, w / (float)h, camNearPlane, camFarPlane);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(m_position);
    viewMatrix.rotate(m_rotation);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    // номер 0 должен совпадать с номером в uniform value qt_Texture0
    m_texture->bind(0);

    // биндим программу, чтоб иметь к ней доступ
    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);
    m_program.setUniformValue("u_modelMatrix", modelMatrix);
    m_program.setUniformValue("u_texture", 0);
    // QVector4D(0.0, 0.0, 0.0, 1.0) - вершина, а не вектор
    m_program.setUniformValue("u_lightColor", m_lightColor);
    m_program.setUniformValue("u_lightPosition", m_lightPosition);
    m_program.setUniformValue("u_lightPower", m_lightPower);
    m_program.setUniformValue("u_specularFactor", m_specularFactor);
    m_program.setUniformValue("u_ambientFactor", m_ambientFactor);

    m_arrayBuffer.bind();

    int offset = 0;

    int vertLoc = m_program.attributeLocation("a_position");
    m_program.enableAttributeArray(vertLoc);
    m_program.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));
    // сдвиг 3д, так как перед текстурными координатами идут координаты вершины
    offset += sizeof(QVector3D);

    int texLoc = m_program.attributeLocation("a_texcoord");
    m_program.enableAttributeArray(texLoc);
    m_program.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    // добавляем нормаль, сдвиг 2д, так как перед координатами нормали идут текстурные координаты
    offset += sizeof(QVector2D);

    int normLoc = m_program.attributeLocation("a_normal");
    m_program.enableAttributeArray(normLoc);
    m_program.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
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

    QVector<GLuint> indexes; //хватило бы и short
    for(int i = 0; i < 24; i += 4)
    {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_arrayBuffer.create();
    m_arrayBuffer.bind();
    m_arrayBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    // освобождаем буфер
    m_arrayBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    // особенность OpenGL, нужно текстуру подавать отраженную по вертикали
    m_texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}


