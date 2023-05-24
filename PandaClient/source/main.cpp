#include "Panda.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Panda w;
    w.show();
    return a.exec();
}
