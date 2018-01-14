#include "contestload.h"
#include "ui_contestload.h"
#include "logging_categories.h"
#include "mainwindow.h"
#include <QDir>

ContestLoad::ContestLoad(QWidget *parent, QStringList contests_configs_list) :
    QDialog(parent),
    ui(new Ui::ContestLoad)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);     // Число колонок        //->addItems(contests_configs_list);
    ui->tableWidget->setShowGrid(true);     // Включить сетку
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);      // Можно выделить только один элемент
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);       // Выделять всю строку
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);   // Растянуть последнюю колонку на всё доступное пространство

    // Установить заголовки таблицы
    QStringList headers;
    headers.append("File name");
    headers.append("Contest name");
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Прочитать названия контестов из файлов-конфигов
    int row = 0;
    QString contestConfigDir = "contests";
    foreach (const QString &contest_config_file_name, contests_configs_list) {

        // Название контеста из файла констест-конфига
        QString fullContestConfigFile =
            QDir::homePath() + QDir::separator() + configDir + QDir::separator() + contestConfigDir + QDir::separator() + contest_config_file_name;
        contestSettings = new QSettings(fullContestConfigFile, QSettings::IniFormat);
        contestSettings->setIniCodec("UTF-8");
        contest_name = contestSettings->value("contest_name");

        // Заполнить строки таблицы
        ui->tableWidget->insertRow(row);
        /* Первая колонка */
        QTableWidgetItem *first_col = new QTableWidgetItem(contest_config_file_name);
        first_col->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, 0, first_col);
        /* Вторая колонка */
        QTableWidgetItem *second_col = new QTableWidgetItem(contest_name.toString());
        second_col->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, 1, second_col);

        row++;
    }
    ui->tableWidget->resizeColumnsToContents();     // Ресайзить колонки по содержимому


    //
//    connect(ui->tableWidget->selectedItems(), SIGNAL( cellChanged(int, int)),);

//
}

ContestLoad::~ContestLoad()
{
    delete ui;
}


// В buttonBox нажата кнопка "OK"
void ContestLoad::on_buttonBox_accepted()
{
    // Получить выбранное имя файла контест-конфига
    QList<QTableWidgetItem *> selected_line_list = ui->tableWidget->selectedItems();
    contest_config_file_name = selected_line_list.first()->text();
    qInfo(logInfo()) << contest_config_file_name;

    // Здесь нужно послать сигнал для чтения контест-конфига


}
