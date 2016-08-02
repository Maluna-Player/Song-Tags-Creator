#include <QApplication>
#include <QDebug>
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.resize(500, 300);
    window.show();

    return a.exec();
}
