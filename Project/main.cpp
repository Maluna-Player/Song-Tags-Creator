#include <QApplication>
#include <QDesktopWidget>
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.resize(500, 300);
    window.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, window.size(), qApp->desktop()->availableGeometry()));

    window.show();

    return a.exec();
}
