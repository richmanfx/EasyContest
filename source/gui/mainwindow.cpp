#include <QDebug>
#include "mainwindow.h"
#include "../logging_categories.h"
#include <string>

MainWindow::MainWindow(QWidget *parent, QString name, QString configDir, QString configFile) :
    QMainWindow(parent), pPlotter(new QCustomPlot) {

    setupUi(this);

    qInfo(logInfo()) << "==================================================";
    qInfo(logInfo()) << tr("Start program");

    // Настройки приложения, читать из INI-файла
    settings = new QSettings(QDir::homePath() + QDir::separator() + configDir + QDir::separator() + configFile, QSettings::IniFormat, this);
    settings->setIniCodec("UTF-8");
    setObjectName(name);
    loadSettings();

    // Прочитать настройки из контест-конфига
    QString contestConfigDir = "contests";
    QString contestConfigFile = "minitest.ec";
    QString fullContestConfigFile =
            QDir::homePath() + QDir::separator() + configDir + QDir::separator() + contestConfigDir + QDir::separator() + contestConfigFile;
    contestSettings = new QSettings(fullContestConfigFile, QSettings::IniFormat);
    contestSettings->setIniCodec("UTF-8");
    loadContestSettings();

    // Горячие клавиши
    helpShortcut = new QGlobalShortcut(this);
    connect(helpShortcut, &QGlobalShortcut::activated, this, &MainWindow::helpAction);
    helpShortcut->setShortcut(QKeySequence("Ctrl+Alt+H"));

    aboutShortcut = new QGlobalShortcut(this);
    connect(aboutShortcut, &QGlobalShortcut::activated, this, &MainWindow::aboutAction);
    aboutShortcut->setShortcut(QKeySequence("Ctrl+Alt+A"));

    exitShortcut = new QGlobalShortcut(this);
    connect(exitShortcut, &QGlobalShortcut::activated, this, &MainWindow::exitAction);
    exitShortcut->setShortcut(QKeySequence("Ctrl+Alt+X"));

    clearCallShortcut = new QGlobalShortcut(this);
    connect(clearCallShortcut, &QGlobalShortcut::activated, this, &MainWindow::clearCall);
    clearCallShortcut->setShortcut(QKeySequence("Alt+Q"));

    // Подключение к ExpertSDR2
    sdrConnect();



    // Соединиться по кнопке "Connect"
//    connect(pbConnect, &QPushButton::clicked, this, &MainWindow::onConnect);

    connect(&m_tciClient, &TciClient::openStatusChanged, this, &MainWindow::onConnectStatus);

//    connect(&m_tciClient, &TciClient::message, teLog, &QTextEdit::append);

//    connect(&m_tciClient.trxState(), &TciTrxState::stoped , [=](){ pbStart->setChecked(false); });

//    connect(pbStart, &QPushButton::toggled, [=](bool state) {
//        if (state)
//            m_tciClient.trxState().start();
//        else
//            m_tciClient.trxState().stop();
//    });
//

    // Начать передачу при нажатии Enter в поле ввода позывного
    connect(callLineEdit, &QLineEdit::returnPressed, [=]() {
        if (!callLineEdit->text().isEmpty())
            m_tciClient.trxState().setMacros(0u, callLineEdit->text());
        callLineEdit->clear();
    });


} // Tnd MainWindow

MainWindow::~MainWindow() {
    saveSettings();     // Сохранить настройки при выходе
}

void MainWindow::saveSettings() {
    // Host для подключения по TCI к программе ExpertSDR
//     settings->setValue("SunSDR2Host", "127.0.0.1");          // Писать не нужно пока, только читать их конфига
    // Zb: s.setValue( "mykey", QString::fromUtf8( "Значение" ) );

    // Cохранение геометрии всех окон
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    settings->setValue("geometry", geometry());
    settings->endGroup();
    settings->endGroup();
}

// Считать настройки из главного INI-файла
void MainWindow::loadSettings() {
    // Хост и порт для подключения по TCI к программе ExpertSDR
    host = settings->value("SunSDR2Host", "127.0.0.1");
    qInfo(logInfo()) << "SunSDR2Host -> " + host.toString();
    port = settings->value("SunSDR2Port", "40001");
    qInfo(logInfo()) << "SunSDR2Port -> " + port.toString();

    // Считать геометрию всех окон из настроек
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(300, 300, 500, 430)).toRect());
    settings->endGroup();
    settings->endGroup();

    // Уровень логирования
    debug_level = settings->value("DebugLevel");
}

// Считать настройки из файла конфигурации контеста
void MainWindow::loadContestSettings() {
    contest_name = contestSettings->value("contest_name");
    tour_count = contestSettings->value("tour_count");
    tour_duration = contestSettings->value("tour_duration");
    valid_bands = contestSettings->value("valid_bands");

    qInfo(logInfo()) << tr("Contest name") + ": " + contest_name.toString();
    qInfo(logInfo()) << tr("Tour count") + ": " + tour_count.toString();
    qInfo(logInfo()) << tr("Tour duration") + ": " + tour_duration.toString();
    qInfo(logInfo()) << tr("Valid bands") + ": " + valid_bands.toString();
}


void MainWindow::sdrConnect() {
    QUrl t_url("ws://" + host.toString() + ":" + port.toString());
    m_tciClient.open(t_url);
}

// Контекстное меню
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu();

    // Help
    QAction *helpAction = new QAction(tr("Help") + " (Ctrl+Alt+H)", this);
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(helpAction()));
    menu->addAction(helpAction);

    // About
    QAction *aboutAction = new QAction(tr("About") + " (Ctrt+Alt+A)", this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutAction()));
    menu->addAction(aboutAction);

    menu->addSeparator();

    // Exit
    QAction *exitAction = new QAction(tr("Exit") + " (Ctr+Alt+X)", this);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitAction()));
    menu->addAction(exitAction);

    menu->exec(QCursor::pos());
}

// Очистить поле ввода позывного
void MainWindow::clearCall() {
    callLineEdit->clear();
}

void MainWindow::onConnect(bool state) {
    state = true;
    if (state) {
        QUrl t_url("ws://" + host.toString() + ":" + port.toString());
        m_tciClient.open(t_url);
        if (debug_level.toString().toLower() == "debug")
            qDebug(logDebug()) << "open";
//        pbConnect->setChecked(true);
    } else {
        m_tciClient.close();
        if (debug_level.toString().toLower() == "debug")
            qDebug(logDebug()) << "close";
//        pbConnect->setChecked(false);
    }
}

void MainWindow::onConnectStatus(bool state) {
    if (state) {
        sbStatus->showMessage(tr("Connected"),2000);
    } else {
//        pbConnect->setChecked(false);
        sbStatus->showMessage(tr("Disconnected"), 2000);
    }
}

// Показать помощь в PDF
void MainWindow::helpAction() {
    QString help_file = QDir::currentPath() +
                        QDir::separator() + "documentation" +
                        QDir::separator() + "EasyContest.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(help_file));
}

// Вывод окна "О программе"
void MainWindow::aboutAction() {
    QMessageBox::about(0, tr("About"), QString(tr(
                "Contest-log program for CW contests<BR>"
                "<FONT COLOR='BLUE'>Version</FONT>: <B><FONT COLOR='RED'>%1</FONT></B>")).arg(VERSION));
}

// Выход из приложения
void MainWindow::exitAction() {
    this->close();
}

// Метод для перемещения беззаголовочного окна
void MainWindow::mouseMoveEvent( QMouseEvent* e ) {
    if( e->buttons() | Qt::LeftButton ) {
        setGeometry(
            pos().x() + ( e->x() - dx ),
            pos().y() + ( e->y() - dy ),
            width(),
            height()
        );
    }
}

// Метод для перемещения беззаголовочного окна
void MainWindow::mousePressEvent( QMouseEvent* e ) {
    if( e->button() == Qt::LeftButton ) {
        dx = e->x();
        dy = e->y();
        setCursor( Qt::OpenHandCursor );
    }
}

// Метод для перемещения беззаголовочного окна
void MainWindow::mouseReleaseEvent( QMouseEvent* e ) {
    if( e->button() == Qt::LeftButton ) {
        setCursor( Qt::ArrowCursor );
    }
}
