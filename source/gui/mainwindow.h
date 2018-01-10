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
#define CONFIG_DIR ".easycontest"       // Ещё такое же значение и в "contestload.h"
#define CONFIG_FILE "ec.cnf"

using namespace ExpertElectronics;

class MainWindow : public QMainWindow, protected Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString name = "MainForm", QString configDir = CONFIG_DIR, QString configFile = CONFIG_FILE);

    void saveSettings();
    void loadSettings();
    void loadContestSettings(QString config_filq_name);
    void sdrConnect(bool);
    void dateShow();
    void timerEvent(QTimerEvent *timer_event);
    void contextMenuEvent();
    void clearCall();
    void clearAllFields();
    void bandShow();
    TciTrxState& trxState();
    ~MainWindow();

private slots:
//    void onConnect(bool state);
//    void onConnectStatus(bool state);

    void onConnected();
    void onClosed();
    void onSendMessage(const QString &message);

    void helpAction();
    void aboutAction();
    void exitAction();
    void loadContest();

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


private:
    QString configDir = CONFIG_DIR;
    Ui::MainWindow *ui;
    QSettings *settings;
    QSettings *contestSettings;

    QCustomPlot *pPlotter;

    TciClient m_tciClient;
    bool       m_open;
    QWebSocket  m_webSocket;
    TciParser   m_parser;
    QMetaObject::Connection m_c1;

    QVariant host;      // Хост и порт для подключения по TCI к программе ExpertSDR
    QVariant port;
    QVariant debug_level;       // Уровень логирования
    QVariant time_shift;        // Сдвиг времени относительно компьютерного, в минутах
    QVariant contests_configs_dir;       // Имя директории с контест-конфигами

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

    // Параметры контеста
    QVariant contest_name;      // Название
    QVariant tour_count;        // Количество минитуров
    QVariant tour_duration;     // Длительность одного минитура в минутах
    QVariant valid_bands;       // Допустимые диапазоны

    // Горячие клавиши
    QGlobalShortcut *helpShortcut;
    QGlobalShortcut *aboutShortcut;
    QGlobalShortcut *exitShortcut;
    QGlobalShortcut *clearCallShortcut;
    QGlobalShortcut *clearAllLineEdit;
    QGlobalShortcut *loadNewContest;


    // Изменения координат при перемещении беззаголовочного окна
    int dx;
    int dy;

    // Идентификатор таймера
    int timerId;

signals:
    void message(QString);
    void openStatusChanged(bool);




};

#endif // MAINWINDOW_H
