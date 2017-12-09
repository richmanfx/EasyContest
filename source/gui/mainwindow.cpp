#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString name) : 
    QMainWindow(parent), pPlotter(new QCustomPlot)
{
    setupUi(this);

    // Настройки приложения, читать из INI-файла
    QString configDir = ".easycontest";
    QString cofigFile = ".ec.cnf";
    settings = new QSettings(QDir::homePath() + QDir::separator() + configDir + QDir::separator() + cofigFile, QSettings::IniFormat, this);
    setObjectName(name);
    loadSettings();
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

MainWindow::~MainWindow()
{
    saveSettings();     // Сохранить настройки при выходе
}

void MainWindow::saveSettings()
{
    // Host для подключения по TCI к программе ExpertSDR
    // settings->setValue("SunSDR2Host", "127.0.0.1");          // Писать не нужно пока, только читать их конфига

    // Cохранение геометрии всех окон
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    settings->setValue("geometry", geometry());
    settings->endGroup();
    settings->endGroup();
}

void MainWindow::loadSettings()
{
    // Хост и порт для подключения по TCI к программе ExpertSDR
    host = settings->value("SunSDR2Host", "127.0.0.1");
    teLog->append("Настройки:\n\tSunSDR2Host -> " + host.toString());
    port = settings->value("SunSDR2Port", "40001");
    teLog->append("\tSunSDR2Port -> " + port.toString());

    // Считать геометрию всех окон из настроек
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(300, 300, 500, 430)).toRect());
    settings->endGroup();
    settings->endGroup();
}

void MainWindow::sdrConnect()
{
    QUrl t_url("ws://" + host.toString() + ":" + port.toString());
    m_tciClient.open(t_url);
}

void MainWindow::onConnect(bool state)
{
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

void MainWindow::onConnectStatus(bool state)
{
    if (state) {
//        sbStatus->showMessage(tr("Connected."), 2000);
        sbStatus->showMessage("Connected");
    } else {
        pbConnect->setChecked(false);
        sbStatus->showMessage(tr("Disconnected."), 2000);
    }
}
