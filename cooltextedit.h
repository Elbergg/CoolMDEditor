#ifndef COOLTEXTEDIT_H
#define COOLTEXTEDIT_H

#include <QMainWindow>
#include <QTextEdit>


struct TextBlock {
    QString htmlVal;
    std::string mdVal;
    int start, end;
};


class CoolTextEdit : public QTextEdit
{
    Q_OBJECT
    std::string refreshBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock);
public:
    QString oldContent;
    QString newContent;
    explicit CoolTextEdit(QWidget* parent = nullptr);
    int getLineNumber();
    void refreshWidget();
    std::pair<int, int> selectEditedText(std::string&);
    CoolTextEdit();
    int selectedBlock;
    QTextCursor coursor;
    std::vector<TextBlock> textBlocks;
};

#endif // COOLTEXTEDIT_H
