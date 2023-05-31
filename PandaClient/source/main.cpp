#include "PandaClient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PandaClient w;
    return a.exec();
}
