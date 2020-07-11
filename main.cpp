#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);
    Plants plants;
    PlantManager plantManager(&plants);
    MainWindow w (&plantManager);
    w.show();
    return a.exec();
}
