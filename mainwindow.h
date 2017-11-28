#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <TciClient>
#include <QCustomPlot>

using namespace ExpertElectronics;

class QTextBrowser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onConnect(bool state);
    void on_infoButton_clicked();       // Объявление слота

private:
    Ui::MainWindow *ui;
    QTextBrowser *textBrowser;
    TciClient m_tciClient;

signals:


};

#endif // MAINWINDOW_H
