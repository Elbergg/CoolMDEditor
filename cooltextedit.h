#ifndef COOLTEXTEDIT_H
#define COOLTEXTEDIT_H

#include <QTextEdit>


class CoolTextEdit : public QTextEdit
{
    Q_OBJECT
    QString sourceText;
public:
    explicit CoolTextEdit(QWidget* parent = nullptr);
    int getLineNumber();
    void refreshWidget();
    CoolTextEdit();
};

#endif // COOLTEXTEDIT_H
