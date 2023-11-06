#include "widget.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QTimer>
#include <QDir>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(".");
    Widget w;
    w.show();
    return a.exec();
}
