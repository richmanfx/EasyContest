#ifndef CONTESTLOAD_H
#define CONTESTLOAD_H

#include <QAbstractButton>
#include <QDialog>
#include <QSettings>

#define CONFIG_DIR ".easycontest" // Ещё такое же значение и в "mainwindow.h"

namespace Ui {
class ContestLoad;
}

class ContestLoad : public QDialog
{
    Q_OBJECT

public:
    explicit ContestLoad(QWidget *parent = 0, QStringList contests_configs_list = QStringList());
    ~ContestLoad();
    QString contest_config_file_name;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ContestLoad *ui;
    QSettings *contestSettings;
    QString configDir = CONFIG_DIR;

    // Параметры контеста
    QVariant contest_name;      // Название

signals:
    void signalFromOkButton();    // Сигнал от кнопки "ОК"

};

#endif // CONTESTLOAD_H
