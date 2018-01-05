#include "customlineedit.h"
#include "logging_categories.h"
#include <QDebug>
#include <QGraphicsEffect>

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent) {
       CustomLineEdit::setMaxLength(10);        // Максимальное количество символов
}

CustomLineEdit::~CustomLineEdit() {

}

// Переопределение события нажатия клавиши - если пробел, то переключить фокус
void CustomLineEdit::keyPressEvent( QKeyEvent *event ) {
    if(event->key() == Qt::Key_Space) {
//        qInfo(logInfo()) << tr("Нажат Пробел!!!");
        CustomLineEdit::focusNextChild();

    } else {
        return QLineEdit::keyPressEvent(event);
    }
}

// Переопределение события получения фокуса
void CustomLineEdit::focusInEvent( QFocusEvent *event ) {

    if(event->type() == QEvent::FocusIn) {
//      qInfo(logInfo()) << tr("Фокус получен!");
      CustomLineEdit::end(false);
      CustomLineEdit::focusOutEvent(event);
    } else {
        return CustomLineEdit::focusInEvent(event);
    }
}
