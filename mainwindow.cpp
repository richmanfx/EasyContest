#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), pPlotter(new QCustomPlot)
{
    ui->setupUi(this);

//    connect(infoButton, &QPushButton::toggled, this, &MainWindow::onConnect);
    connect(ui->infoButton, &QPushButton::toggled, this, &MainWindow::onConnect);

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
    delete ui;
}

// Обращение к слоту
void MainWindow::on_infoButton_clicked()
{
    ui->textBrowser->append("Привет!");

}

void MainWindow::onConnect(bool state)
{
    ui->textBrowser->append("Привет из 'onConnect'.");


    if (state) {
        QUrl t_url("ws://127.0.0.1:40001");
        m_tciClient.open(t_url);
        ui->textBrowser->append("open");
    }
    else {
        m_tciClient.close();
        ui->textBrowser->append("close");
    }
}
