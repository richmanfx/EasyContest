#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <TciClient>
#include <QCustomPlot>

#include "ui_mainwindow.h"

using namespace ExpertElectronics;

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString name = "MainForm");
    void saveSettings();
    void loadSettings();
    void sdrConnect();
    ~MainWindow();

private slots:
    void onConnect(bool state);
    void onConnectStatus(bool state);

private:
    Ui::MainWindow *ui;
    QSettings *settings;

    QCustomPlot *pPlotter;
    TciClient m_tciClient;

    QMetaObject::Connection m_c1;

    QVariant host;      // Хост и порт для подключения по TCI к программе ExpertSDR
    QVariant port;

signals:


};

#endif // MAINWINDOW_H
