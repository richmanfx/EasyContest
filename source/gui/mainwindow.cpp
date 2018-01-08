#include <QDebug>
#include "mainwindow.h"
#include "../logging_categories.h"
#include "../customlineedit.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "contestload.h"

MainWindow::MainWindow(QWidget *parent, QString name, QString configDir, QString configFile) :
    QMainWindow(parent) {

    setupUi(this);

    qInfo(logInfo()) << "=====================================================";
    qInfo(logInfo()) << tr("Start program");

    // Настройки приложения, читать из INI-файла
    settings = new QSettings(QDir::homePath() + QDir::separator() + configDir + QDir::separator() + configFile, QSettings::IniFormat, this);
    settings->setIniCodec("UTF-8");
    setObjectName(name);
    loadSettings();

    // Выставить параметры шрифтов
    QPalette *palette = new QPalette();
    QFont field_font;
    field_font.setFamily("Monospace");

    // Поле ввода позывного - параметры шрифтов
    palette->setColor(QPalette::Text, call_font_color.toString());
    callLineEdit->setPalette(*palette);
    field_font.setBold(call_font_bold.toBool());
    field_font.setPixelSize(call_font_size.toInt());
    callLineEdit->setFont(field_font);

    // Поле принимаемого номера - параметры шрифтов
    palette->setColor(QPalette::Text, rx_num_font_color.toString());
    numberLineEdit->setPalette(*palette);
    field_font.setBold(num_font_bold.toBool());
    field_font.setPixelSize(num_font_size.toInt());
    numberLineEdit->setFont(field_font);

    // Поле-лейбл передаваемого номера - параметры шрифтов
    palette->setColor(QPalette::Base, "lightGray");
    palette->setColor(QPalette::Text, tx_num_font_color.toString());
    field_font.setBold(num_font_bold.toBool());
    field_font.setPixelSize(num_font_size.toInt());
    tx_num_label->setPalette(*palette); tx_num_label->setFont(field_font);

    // Лейблы - параметры шрифтов
    field_font.setBold(label_font_bold.toBool());
    field_font.setPixelSize(label_font_size.toInt());
    band_label->setFont(field_font);
    cw_speed_label->setFont(field_font);
    CQ_SP_label->setFont(field_font);
    date_label->setFont(field_font);
    time_label->setFont(field_font);
    auto_CQ_label->setFont(field_font);



    /***** Горячие клавиши ******/
    // Помощь в PDF-формате
    helpShortcut = new QGlobalShortcut(this);
    connect(helpShortcut, &QGlobalShortcut::activated, this, &MainWindow::helpAction);
    helpShortcut->setShortcut(QKeySequence("Ctrl+Alt+H"));
    // О программе
    aboutShortcut = new QGlobalShortcut(this);
    connect(aboutShortcut, &QGlobalShortcut::activated, this, &MainWindow::aboutAction);
    aboutShortcut->setShortcut(QKeySequence("Ctrl+Alt+A"));
    // Выход из программы
    exitShortcut = new QGlobalShortcut(this);
    connect(exitShortcut, &QGlobalShortcut::activated, this, &MainWindow::exitAction);
    exitShortcut->setShortcut(QKeySequence("Ctrl+Alt+X"));
    // Очистить поле ввода позывного
    clearCallShortcut = new QGlobalShortcut(this);
    connect(clearCallShortcut, &QGlobalShortcut::activated, this, &MainWindow::clearCall);
    clearCallShortcut->setShortcut(QKeySequence("Alt+Q"));
    // Очистить все поля ввода
    clearAllLineEdit = new QGlobalShortcut(this);
    connect(clearAllLineEdit, &QGlobalShortcut::activated, this, &MainWindow::clearAllFields);
    clearAllLineEdit->setShortcut(QKeySequence("Alt+W"));
    // Закгрузить новый контест
    loadNewContest = new QGlobalShortcut(this);
    connect(loadNewContest, &QGlobalShortcut::activated, this, &MainWindow::loadContest);
    loadNewContest->setShortcut(QKeySequence("Ctrl+O"));


    // Вывести время сразу при старте программы
    QTime current_time = QTime::currentTime();
    current_time = current_time.addSecs(time_shift.toInt() * 60);   // Из минут в секунды
    time_label->setText(current_time.toString("hh:mm"));

    // Запустить таймер для периодического обновления времени
    timerId = startTimer(10000);    // 1 раз в 10 секунд

    // Подключение к ExpertSDR2
    sdrConnect(true);

    // Вывести дату
    dateShow();

    // Вывести на лейбл текущий диапазон из SDR
    bandShow();


    //================================================================================================================//
    // Соединиться по кнопке "Connect"
//    connect(pbConnect, &QPushButton::clicked, this, &MainWindow::onConnect);

//    connect(&m_tciClient, &TciClient::openStatusChanged, this, &MainWindow::onConnectStatus);

//    connect(&m_tciClient, &TciClient::message, teLog, &QTextEdit::append);
    connect(&m_parser, &TciParser::message, this, &MainWindow::onSendMessage);

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
        if (!callLineEdit->text().isEmpty()) {
            if (!numberLineEdit->text().isEmpty()) {
                m_tciClient.trxState().setMacros(0u, callLineEdit->text());
                clearAllFields();
                callLineEdit->setFocus();
//                callLineEdit->end(false);
            }
        }
    });

    // Начать передачу при нажатии Enter в поле ввода номера
    connect(numberLineEdit, &QLineEdit::returnPressed, [=]() {
        if (!callLineEdit->text().isEmpty()) {
            m_tciClient.trxState().setMacros(0u, callLineEdit->text());
            clearAllFields();
            callLineEdit->setFocus();
        }
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

    // Уровень логирования
    debug_level = settings->value("DebugLevel");

    // Сдвиг времени в часах относително компьютерного
    time_shift = settings->value("TimeShift");

    // Имя директории с контест-конфигами
    contests_configs_dir = settings->value("ContestsConfigsDir");

    // Считать геометрию всех окон из настроек
    settings->beginGroup("Windows");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(300, 300, 500, 430)).toRect());
    settings->endGroup();
    settings->endGroup();

    // Цвет и размер шрифтов
    settings->beginGroup("Fonts");
    call_font_color = settings->value("CallFontColor");   // Цвет шрифта позывного
    call_font_size = settings->value("CallFontSize");     // Размер шрифта позывного
    call_font_bold = settings->value("CallFontBold");     // Жирность шрифта позывного

    num_font_size = settings->value("NumFontSize");      // Размер шрифта номера
    num_font_bold = settings->value("NumFontBold");      // Жирность шрифта номера
    rx_num_font_color = settings->value("RxNumFontColor");    // Цвет шрифта RX номера
    tx_num_font_color = settings->value("TxNumFontColor");    // Цвет шрифта TX номера

    label_font_size = settings->value("LabelFontSize");        // Размер шрифта лейблов
    label_font_bold = settings->value("LabelFontBold");        // Жирность шрифта лейблов
    settings->endGroup();


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


// Подключиться к SDR
void MainWindow::sdrConnect(bool state) {

    if (state) {
        QUrl t_url("ws://" + host.toString() + ":" + port.toString());
        m_tciClient.open(t_url);
    } else {
        m_tciClient.close();
    }
}

// Вывести на лейбл текущий диапазон из SDR
void MainWindow::bandShow() {

    // Пока только один первый trx используем
    quint32 trx = 0u;

    // Состояние SDR
    TciTrxState& sdr_state = m_tciClient.trxState();

    // Установить скорость CW ==> Не работает пока
    m_tciClient.trxState().setCwSpeed(50);

    QString cw_speed = QString::number(sdr_state.cwSpeed());
    cw_speed_label->setText(cw_speed);

    emit message(QStringLiteral("STOP;"));         //+ QString::number(trx) + QStringLiteral(",") + QStringLiteral("0,3500000") + QStringLiteral(";"));

                 // QString::number(static_cast<qint64>(sdr_state.ddsFreq(trx))) +
//                 QStringLiteral(";"));

    QString sdr_freq = QString::number(static_cast<qint64>(sdr_state.ddsFreq(trx)));        // DDS частота


//    QString is_start = QString::number(sdr_state.isStart());

    band_label->setText(sdr_freq);
}

// Вывести на лейбл текущую дату
void MainWindow::dateShow() {
    QDateTime current_date = QDateTime::currentDateTime();
    current_date = current_date.addSecs(time_shift.toInt() * 60);   // Из минут в секунды
    date_label->setText(current_date.toString("yyyy.MM.dd"));
}

// Вывести на лейбл текущее время
void MainWindow::timerEvent(QTimerEvent *timer_event) {
    if (timer_event->timerId() == timerId) {
        QTime current_time = QTime::currentTime();
//        qInfo(logInfo()) << "current_time: " << current_time;
        current_time = current_time.addSecs(time_shift.toInt() * 60);   // Из минут в секунды
        time_label->setText(current_time.toString("hh:mm"));
        dateShow();
    }
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

    // Load Contest
    QAction *loadContest = new QAction(tr("Load Contest") + " (Ctrt+O)", this);
    connect(loadContest, SIGNAL(triggered(bool)), this, SLOT(loadContest()));
    menu->addAction(loadContest);

    // Разделитель
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

// Очистить все поля ввода
void MainWindow::clearAllFields() {
    callLineEdit->clear();
    numberLineEdit->clear();
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

// Загрузить новый контест из списка конфигурационных файлов контестов
void MainWindow::loadContest() {
    qInfo(logInfo()) << "Загрузка нового контеста";

    // Получить список файлов из директории контест-конфигов
    QDir contest_dir(QDir::homePath() + QDir::separator() + configDir + QDir::separator() + contests_configs_dir.toString());
    QStringList files_mask; files_mask << "*.ec";
    QStringList contests_configs_list = contest_dir.entryList(files_mask);

    // Вывести список контест-конфигов в новую форму для выбора
    /* Открыть новую форму с "QListWidget" */
    ContestLoad *w2 = new ContestLoad(this, contests_configs_list);
    w2->setWindowTitle(tr("Contest load"));
    w2->show();

    /* По кнопке "Выбрать" или двойному щелчку ЛКМ получить имя файла контест-конфиге */

    /* Прочитать настройки из контест-конфига */

//    QString contestConfigDir = "contests";
//    QString contestConfigFile = "minitest.ec";
//    QString fullContestConfigFile =
//            QDir::homePath() + QDir::separator() + configDir + QDir::separator() + contestConfigDir + QDir::separator() + contestConfigFile;
//    contestSettings = new QSettings(fullContestConfigFile, QSettings::IniFormat);
//    contestSettings->setIniCodec("UTF-8");
//    loadContestSettings();

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

void MainWindow::onSendMessage(const QString &message)
{
    m_webSocket.sendTextMessage(message);
}

void MainWindow::onConnected()
{
    m_open = true;
    emit openStatusChanged(m_open);
}

void MainWindow::onClosed()
{
    m_open = false;
    emit openStatusChanged(m_open);
}

