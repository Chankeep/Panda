#ifndef PANDA_H
#define PANDA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Panda; }
QT_END_NAMESPACE

class Panda : public QMainWindow
{
    Q_OBJECT

public:
    Panda(QWidget *parent = nullptr);
    ~Panda();

private:
    Ui::Panda *ui;
};
#endif // PANDA_H
