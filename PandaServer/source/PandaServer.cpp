#include "PandaServer.h"
#include "ui_PandaServer.h"

PandaServer::PandaServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PandaServer)
{
    ui->setupUi(this);
}

PandaServer::~PandaServer()
{
    delete ui;
}

