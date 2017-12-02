#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
//#include <TciClient>
//#include <QCustomPlot>

//using namespace ExpertElectronics;

class QTextBrowser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString name = "MainForm");
    ~MainWindow();

public:
    void saveSettings();
    void loadSettings();

private slots:
//    void onConnect(bool state);
    void on_infoButton_clicked();       // Объявление слота

private:
    Ui::MainWindow *ui;
    QTextBrowser *textBrowser;
    QSettings *settings;
//    TciClient m_tciClient;
//    QCustomPlot *pPlotter;

signals:


};

#endif // MAINWINDOW_H
