#ifndef COOLTEXTEDIT_H
#define COOLTEXTEDIT_H

#include <QMainWindow>
#include <QTextEdit>


class CoolTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    QString sourceText;
    explicit CoolTextEdit(QWidget* parent = nullptr);
    int getLineNumber();
    void refreshWidget();
    std::pair<int, int> selectEditedText();
    CoolTextEdit();
};

#endif // COOLTEXTEDIT_H
