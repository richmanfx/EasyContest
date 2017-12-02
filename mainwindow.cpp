#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString name) : QMainWindow(parent), ui(new Ui::MainWindow)     //, pPlotter(new QCustomPlot)
{
    ui->setupUi(this);

    // Настройки приложения, сохранять в INI-файл
    settings = new QSettings(QDir::homePath() + QDir::separator() + ".ec.conf", QSettings::IniFormat, this);
    setObjectName(name);
    loadSettings();

//    connect(infoButton, &QPushButton::toggled, this, &MainWindow::onConnect);
//    connect(ui->infoButton, &QPushButton::toggled, this, &MainWindow::onConnect);

//    connect(&m_tciClient.trxState(), &TciTrxState::started, [=](){ ui->startButton->setChecked(true);  });
//    connect(&m_tciClient.trxState(), &TciTrxState::stoped , [=](){ ui->startButton->setChecked(false); });
//    connect(ui->startButton, &QPushButton::toggled, [=](bool state) {
//        if (state)
//            m_tciClient.trxState().start();
//        else
//            m_tciClient.trxState().stop();
//    });

}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::saveSettings()
{
    // Host для подключения по TCI к программе ExpertSDR
    settings->setValue("SunSDR2Host", "127.0.0.1");

    // Cохранение геометрии всех окон
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    settings->setValue("geometry", geometry());
    settings->endGroup();
    settings->endGroup();
}

void MainWindow::loadSettings()
{
    // Параметр и значение по умолчанию - Host для подключения по TCI к программе ExpertSDR
    ui->textBrowser->append("Настройки: SunSDR2Host -> " + settings->value("SunSDR2Host", "127.0.0.1").toString());

    // Считать геометрию всех окон из настроек
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(200, 200, 300, 300)).toRect());
    settings->endGroup();
    settings->endGroup();
}

// Обращение к слоту
void MainWindow::on_infoButton_clicked()
{
    ui->textBrowser->append("Привет!");

}

//void MainWindow::onConnect(bool state)
//{
//    ui->textBrowser->append("Привет из 'onConnect'.");


//    if (state) {
//        QUrl t_url("ws://127.0.0.1:40001");
//        //m_tciClient.open(t_url);
//        ui->textBrowser->append("open");
//    }
//    else {
//        //m_tciClient.close();
//        ui->textBrowser->append("close");
//    }
//}
