#ifndef PANDASERVER_H
#define PANDASERVER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PandaServer; }
QT_END_NAMESPACE

class PandaServer : public QMainWindow
{
    Q_OBJECT

public:
    PandaServer(QWidget *parent = nullptr);
    ~PandaServer();

private:
    Ui::PandaServer *ui;
};
#endif // PANDASERVER_H
