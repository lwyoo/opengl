#include "mainwindow.h"

#include <QApplication>
#include <QtQuick/QQuickView>

#include "squircle.h"

int main(int argc, char *argv[])
{   
    QApplication app(argc, argv);

    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

    MainWindow w;
    w.show();
    return app.exec();
}
