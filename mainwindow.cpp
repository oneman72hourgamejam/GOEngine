#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_camAngle_valueChanged(double arg1)
{
    this->ui->widget->camAngle = arg1;
    qDebug() << "camAngle" << arg1;
}

void MainWindow::on_camNearPlane_valueChanged(double arg1)
{
    this->ui->widget->camNearPlane = arg1;
    qDebug() << "camNearPlane" << arg1;
}

void MainWindow::on_camFarPlane_valueChanged(double arg1)
{
    this->ui->widget->camFarPlane = arg1;
    qDebug() << "camFarPlane" << arg1;
}

void MainWindow::on_pushButton_2_pressed()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );
    imageObject = new QImage();
    imageObject->load(imagePath);

    this->ui->widget->m_texture = new QOpenGLTexture(imageObject->mirrored());
    this->ui->widget->m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    this->ui->widget->m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    this->ui->widget->m_texture->setWrapMode(QOpenGLTexture::Repeat);

    qDebug() << "imagePath" << imagePath;
}

void MainWindow::on_camSmooth_valueChanged(double arg1)
{
    this->ui->widget->camSmooth = arg1;
    qDebug() << "camSmooth" << arg1;
}

void MainWindow::on_camPosX_valueChanged(double arg1)
{
    this->ui->widget->camPosX = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "camPosX" << arg1;
}

void MainWindow::on_camPosY_valueChanged(double arg1)
{
    this->ui->widget->camPosY = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "camPosY" << arg1;
}

void MainWindow::on_camPosZ_valueChanged(double arg1)
{
    this->ui->widget->camPosZ = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "camPosZ" << arg1;
}
