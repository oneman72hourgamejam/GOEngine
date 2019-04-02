#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    // 16 семплов при отрисовке
    format.setSamples(16);
    // размер буфера глубины, в основном для андроида
    // если не установить - отрисовка сзади, которая должна быть невидима - накладывается на то что отрисовано спереди и перекрывает
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return a.exec();
}
