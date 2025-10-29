#ifndef COOLTEXTEDIT_H
#define COOLTEXTEDIT_H
# pragma once
#include <QMainWindow>
#include <QTextEdit>
struct TextBlock {
    QString htmlVal;
    std::string mdVal;
    int start, end;
};

class CoolBlockBrowser;


class CoolTextEdit : public QTextEdit
{
    Q_OBJECT
    CoolBlockBrowser* browser;

    std::string oldContent;
    std::string newContent;
    std::string refreshBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock);
    int getTextDiffLen();
    void highlightText(int begin, int end);
    std::string renderBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock);
public:
    std::string mdVal;
    QString htmlVal;
    QSize sizeHint() const override {
        int docHeight = static_cast<int>(document()->size().height());
        QMargins margins = contentsMargins();
        int height = docHeight + margins.top() + margins.bottom() + 10;
        return QSize(width(), height);
    }
    explicit CoolTextEdit(CoolBlockBrowser* parent);
    CoolTextEdit(CoolBlockBrowser* parent, QString htmlVal, std::string mdVal);
    int getLineNumber();
    void refreshWidget();
    std::pair<int, int> selectEditedText(std::string&);
    CoolTextEdit();
    int selectedBlock = 0;
    int pos = 0;
    std::vector<TextBlock> textBlocks;

    signals:
        void onTextChanged();

private slots:
    void adjustHeight() {
    int docHeight = static_cast<int>(document()->size().height());
    QMargins margins = contentsMargins();
    int totalHeight = docHeight + margins.top() + margins.bottom() + 10;
    setFixedHeight(totalHeight);
}
    void handleTextChanged();
};

#endif // COOLTEXTEDIT_H
