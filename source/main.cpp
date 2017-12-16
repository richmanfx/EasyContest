#include <QApplication>
#include <QTranslator>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("translations/EasyContest");
    qApp->installTranslator(&translator);

    MainWindow w(0, "EasyContest" );

    // Только кнопки окна "свернуть" и "закрыть"
    w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    // Настройки
    QApplication::setOrganizationName("R5AM");
    QApplication::setApplicationName("EasyContest");

    w.show();

    return a.exec();
}
