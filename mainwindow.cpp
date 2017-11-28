#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), pPlotter(new QCustomPlot)
{
    ui->setupUi(this);

    connect(pbConnect, &QPushButton::toggled, this, &MainWindow::onConnect);
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
    pbRx1StartAudio->setChecked(false);

    if (state) {
        QUrl t_url("ws://" + leAddress->text() + ":" + QString::number(sbPort->value()));
        m_tciClient.open(t_url);
    }
    else {
        m_tciClient.close();
    }
}
