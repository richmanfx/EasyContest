#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <TciClient>
#include <QCustomPlot>
#include <QDesktopServices>
#include "ui_mainwindow.h"
#include "qglobalshortcut.h"
#include "../customlineedit.h"

#define VERSION "0.1.0"

using namespace ExpertElectronics;

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(
            QWidget *parent = 0, QString name = "MainForm",
            QString configDir = ".easycontest", QString configFile = "ec.cnf");
    void saveSettings();
    void loadSettings();
    void loadContestSettings();
    void sdrConnect();
    void contextMenuEvent(QContextMenuEvent *event);
    void clearCall();
    void clearAllFields();
    ~MainWindow();

private slots:
    void onConnect(bool state);
    void onConnectStatus(bool state);

    void helpAction();
    void aboutAction();
    void exitAction();
//    void nextLineEdit(CustomLineEdit *customLineEdit);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QSettings *contestSettings;

    QCustomPlot *pPlotter;
    TciClient m_tciClient;

    QMetaObject::Connection m_c1;

    QVariant host;      // Хост и порт для подключения по TCI к программе ExpertSDR
    QVariant port;
    QVariant debug_level;       // Уровень логирования

    // Цвет и размер шрифтов
    QVariant call_font_color;
    QVariant call_font_size;
    QVariant call_font_bold;
    QVariant num_font_size;
    QVariant num_font_bold;
    QVariant rx_num_font_color;
    QVariant tx_num_font_color;
    QVariant label_font_size;
    QVariant label_font_bold;

    QVariant contest_name;      // Название контеста
    QVariant tour_count;        // Количество минитуров
    QVariant tour_duration;     // Длительность одного минитура в минутах
    QVariant valid_bands;       // Допустимые диапазоны

    // Горячие клавиши
    QGlobalShortcut *helpShortcut;
    QGlobalShortcut *aboutShortcut;
    QGlobalShortcut *exitShortcut;
    QGlobalShortcut *clearCallShortcut;
    QGlobalShortcut *clearAllLineEdit;

    // Изменения координат при перемещении беззаголовочного окна
    int dx;
    int dy;

signals:


};

#endif // MAINWINDOW_H
