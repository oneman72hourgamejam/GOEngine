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

void MainWindow::on_lightColorR_valueChanged(double arg1)
{
    this->ui->widget->lightColorR = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightColorR" << arg1;
}

void MainWindow::on_lightColorG_valueChanged(double arg1)
{
    this->ui->widget->lightColorG = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightColorG" << arg1;
}

void MainWindow::on_lightColorB_valueChanged(double arg1)
{
    this->ui->widget->lightColorB = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightColorB" << arg1;
}

void MainWindow::on_lightColorA_valueChanged(double arg1)
{
    this->ui->widget->lightColorA = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightColorA" << arg1;
}

void MainWindow::on_lightPosX_valueChanged(double arg1)
{
    this->ui->widget->lightPosX = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightPosX" << arg1;
}

void MainWindow::on_lightPosY_valueChanged(double arg1)
{
    this->ui->widget->lightPosY = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightPosY" << arg1;
}

void MainWindow::on_lightPosZ_valueChanged(double arg1)
{
    this->ui->widget->lightPosZ = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightPosZ" << arg1;
}

void MainWindow::on_lightPosW_valueChanged(double arg1)
{
    this->ui->widget->lightPosW = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightPosW" << arg1;
}

void MainWindow::on_lightPower_valueChanged(double arg1)
{
    this->ui->widget->lightPower = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "lightPower" << arg1;
}

void MainWindow::on_specularFactor_valueChanged(double arg1)
{
    this->ui->widget->specularFactor = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "specularFactor" << arg1;
}

void MainWindow::on_ambientFactor_valueChanged(double arg1)
{
    this->ui->widget->ambientFactor = arg1;
    this->ui->widget->updateCamPos();
    qDebug() << "ambientFactor" << arg1;
}
