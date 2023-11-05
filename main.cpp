#include "widget.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

int main(int argc, char *argv[])
{
//    QTimer *ti = new QTimer();
//    ti->start(-1);
//    return 0;
//    QElapsedTimer t;
//    t.start();
//    for(int i = 1; i <= 10; ++i) {
//        qDebug() << t.elapsed() << " ";
//        t.restart();
//        QThread::sleep(1);
//    }
//    return 0;
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
