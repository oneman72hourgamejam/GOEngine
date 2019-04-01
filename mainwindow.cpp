#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>

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
