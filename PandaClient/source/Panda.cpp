#include "Panda.h"
#include "ui_Panda.h"

Panda::Panda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Panda)
{
    ui->setupUi(this);
}

Panda::~Panda()
{
    delete ui;
}

