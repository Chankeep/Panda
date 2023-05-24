#include "PandaServer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PandaServer w;
    w.show();
    return a.exec();
}
