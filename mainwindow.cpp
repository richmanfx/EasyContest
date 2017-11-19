#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



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
