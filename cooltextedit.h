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
    std::string oldContent;
    std::string newContent;
    std::string refreshBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock);
    int getTextDiffLen();
    void highlightText(int begin, int end);
    std::string renderBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock);
public:
    explicit CoolTextEdit(QWidget* parent = nullptr);
    int getLineNumber();
    void refreshWidget();
    std::pair<int, int> selectEditedText(std::string&);
    CoolTextEdit();
    int selectedBlock = 0;
    int pos = 0;
    std::vector<TextBlock> textBlocks;
};

#endif // COOLTEXTEDIT_H
