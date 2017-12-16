#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <TciClient>
#include <QCustomPlot>
#include <QDesktopServices>
#include "ui_mainwindow.h"

#define VERSION "0.1.0"

using namespace ExpertElectronics;

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString name = "MainForm");
    void saveSettings();
    void loadSettings();
    void loadContestSettings();
    void sdrConnect();
    void contextMenuEvent(QContextMenuEvent *event);
    ~MainWindow();

private slots:
    void onConnect(bool state);
    void onConnectStatus(bool state);
    void helpAction();
    void aboutAction();
    void exitAction();


private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QSettings *contestSettings;

    QCustomPlot *pPlotter;
    TciClient m_tciClient;

    QMetaObject::Connection m_c1;

    QVariant host;      // Хост и порт для подключения по TCI к программе ExpertSDR
    QVariant port;

    QVariant contest_name;      // Название контеста
    QVariant tour_count;        // Количество минитуров
    QVariant tour_duration;     // Длительность одного минитура в минутах
    QVariant valid_bands;       // Допустимые диапазоны

signals:


};

#endif // MAINWINDOW_H
