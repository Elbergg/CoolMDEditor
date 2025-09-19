#ifndef COOLTEXTEDIT_H
#define COOLTEXTEDIT_H

#include <QMainWindow>
#include <QTextEdit>


class CoolTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    QString oldContent;
    QString newContent;
    explicit CoolTextEdit(QWidget* parent = nullptr);
    int getLineNumber();
    void refreshWidget();
    std::pair<int, int> selectEditedText(std::string&);
    CoolTextEdit();
};

#endif // COOLTEXTEDIT_H
