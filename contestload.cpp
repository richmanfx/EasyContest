#include "contestload.h"
#include "ui_contestload.h"

ContestLoad::ContestLoad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContestLoad)
{
    ui->setupUi(this);

}

ContestLoad::~ContestLoad()
{
    delete ui;
}
