#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = 0);
    ~CustomLineEdit();

signals:

protected:
    void keyPressEvent(QKeyEvent * event);
    void focusInEvent(QFocusEvent * event);
};

#endif // CUSTOMLINEEDIT_H
