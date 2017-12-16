#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QMessageLogContext>
#include "gui/mainwindow.h"

// Указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

// Объявляение обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

QString configDir = ".easycontest";     // Директория для конфигурационных файлов
QString configFile = "ec.cnf";          // Основной конфигурационный файл
QString logFile = "ec.log";             // Файл лога


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Интернационализация
    QTranslator translator;
    translator.load("translations/EasyContest");
    qApp->installTranslator(&translator);

    // Файл логирования
    QString fullLogFile = QDir::homePath() + QDir::separator() + configDir + QDir::separator() + logFile;
    m_logFile.reset(new QFile(fullLogFile));
    // Открыть файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Установить обработчик
    qInstallMessageHandler(messageHandler);


    MainWindow w(0, "EasyContest", configDir, configFile);

    // Только кнопки окна "свернуть" и "закрыть"
    w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    // Настройки
    QApplication::setOrganizationName("R5AM");
    QApplication::setApplicationName("EasyContest");

    w.show();

    return a.exec();
}

// Реализация обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Очищаем буферизированные данные
}
