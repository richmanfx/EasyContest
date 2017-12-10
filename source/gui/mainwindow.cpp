#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString name) : 
    QMainWindow(parent), pPlotter(new QCustomPlot) {
    setupUi(this);

    // Настройки приложения, читать из INI-файла
    QString configDir = ".easycontest";
    QString configFile = "ec.cnf";
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

    // Подключение к ExpertSDR2
    sdrConnect();



    // Соединиться по кнопке "Connect"
    connect(pbConnect, &QPushButton::clicked, this, &MainWindow::onConnect);

    connect(&m_tciClient, &TciClient::openStatusChanged, this, &MainWindow::onConnectStatus);

    connect(&m_tciClient, &TciClient::message, teLog, &QTextEdit::append);

//    connect(&m_tciClient.trxState(), &TciTrxState::stoped , [=](){ pbStart->setChecked(false); });

//    connect(pbStart, &QPushButton::toggled, [=](bool state) {
//        if (state)
//            m_tciClient.trxState().start();
//        else
//            m_tciClient.trxState().stop();
//    });


}

MainWindow::~MainWindow() {
    saveSettings();     // Сохранить настройки при выходе
}

void MainWindow::saveSettings() {
    // Host для подключения по TCI к программе ExpertSDR
    // settings->setValue("SunSDR2Host", "127.0.0.1");          // Писать не нужно пока, только читать их конфига
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
    teLog->append("Настройки:\n\tSunSDR2Host -> " + host.toString());
    port = settings->value("SunSDR2Port", "40001");
    teLog->append("\tSunSDR2Port -> " + port.toString() + "\n");

    // Считать геометрию всех окон из настроек
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(300, 300, 500, 430)).toRect());
    settings->endGroup();
    settings->endGroup();
}

// Считать настройки из файла конфигурации контеста
void MainWindow::loadContestSettings() {
    contest_name = contestSettings->value("contest_name");
    tour_count = contestSettings->value("tour_count");
    tour_duration = contestSettings->value("tour_duration");
    valid_bands = contestSettings->value("valid_bands");

    teLog->append("Настройки контеста:\n\t"
                  + contest_name.toString() + "\n\t "
                  + tour_count.toString() + "\n\t "
                  + tour_duration.toString() + "\n\t "
                  + valid_bands.toString() + "\n");
}


void MainWindow::sdrConnect() {
    QUrl t_url("ws://" + host.toString() + ":" + port.toString());
    m_tciClient.open(t_url);
}

// Контекстное меню
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu();

    QAction *aboutAction = new QAction("About", this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutAction()));
//    menu->addAction("&About", this, SLOT(aboutAction()), Qt::CTRL + Qt::ALT + Qt::Key_A);
//    QShortcut* pAboutShortcut = new QShortcut(Qt::CTRL + Qt::Key_A, this, SLOT(aboutAction()));
    menu->addAction(aboutAction);

    menu->addSeparator();

    QAction *exitAction = new QAction("Exit", this);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitAction()));
//    menu->addAction("&Exit", this, SLOT(exitAction()), Qt::CTRL + Qt::ALT + Qt::Key_X);
//    QShortcut* pAboutShortcut = new QShortcut(Qt::CTRL + Qt::Key_A, this, SLOT(aboutAction()));
//    exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_X));
//    exitAction->setShortcut(tr("Ctrl+X"));
    menu->addAction(exitAction);

    menu->exec(QCursor::pos());
}

void MainWindow::onConnect(bool state) {
    state = true;
    if (state) {
        QUrl t_url("ws://" + host.toString() + ":" + port.toString());
        m_tciClient.open(t_url);
        teLog->append("open");
        pbConnect->setChecked(true);
    } else {
        m_tciClient.close();
        teLog->append("state =" + state);
        teLog->append("close");
        pbConnect->setChecked(false);
    }
}

void MainWindow::onConnectStatus(bool state) {
    if (state) {
//        sbStatus->showMessage(tr("Connected."), 2000);
        sbStatus->showMessage("Connected");
    } else {
        pbConnect->setChecked(false);
        sbStatus->showMessage(tr("Disconnected."), 2000);
    }
}

// Выход из приложения
void MainWindow::exitAction()
{
    this->close();
}

// Вывод окна "О программе"
void MainWindow::aboutAction()
{
    QMessageBox::about(0, "About", QString("Программа для CW контестов\n\nVersion: %1\n\n").arg(VERSION));
}
