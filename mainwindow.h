#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_camAngle_valueChanged(double arg1);

    void on_camNearPlane_valueChanged(double arg1);

    void on_camFarPlane_valueChanged(double arg1);

    void on_pushButton_2_pressed();

    void on_camSmooth_valueChanged(double arg1);

    void on_camPosX_valueChanged(double arg1);

    void on_camPosY_valueChanged(double arg1);

    void on_camPosZ_valueChanged(double arg1);

    void on_lightColorR_valueChanged(double arg1);

    void on_lightColorG_valueChanged(double arg1);

    void on_lightColorB_valueChanged(double arg1);

    void on_lightColorA_valueChanged(double arg1);

    void on_lightPosX_valueChanged(double arg1);

    void on_lightPosY_valueChanged(double arg1);

    void on_lightPosZ_valueChanged(double arg1);

    void on_lightPower_valueChanged(double arg1);

    void on_specularFactor_valueChanged(double arg1);

    void on_ambientFactor_valueChanged(double arg1);

    void on_lightPosW_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QPixmap image;
    QImage  *imageObject;
};

#endif // MAINWINDOW_H
