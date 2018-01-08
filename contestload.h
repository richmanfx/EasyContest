#ifndef CONTESTLOAD_H
#define CONTESTLOAD_H

#include <QDialog>

namespace Ui {
class ContestLoad;
}

class ContestLoad : public QDialog
{
    Q_OBJECT

public:
    explicit ContestLoad(QWidget *parent = 0);
    ~ContestLoad();

private:
    Ui::ContestLoad *ui;
};

#endif // CONTESTLOAD_H
