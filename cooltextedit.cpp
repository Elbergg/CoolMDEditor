#include "cooltextedit.h"
#include "coolmd.h"
#include <QTextEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QLabel>
#include <string>
#include <sstream>
CoolTextEdit::CoolTextEdit() {}

CoolTextEdit::CoolTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

int CoolTextEdit::getLineNumber(){
    QTextCursor cursor = this->textCursor();
    int position = cursor.position();
    QTextDocument *doc = this->document();
    QTextBlock block = doc->findBlock(position);
    return block.blockNumber();
}


std::pair<std::string, std::string> excludeLines(std::string& content, int lineNum)
{
    int n = 0;
    std::istringstream iss(content);
    std::string editedLine;
    std::string rest;
    for(std::string line; std::getline(iss, line);)
    {
        if(n == lineNum)
        {
            editedLine = line;
            editedLine += '\n';
        }
        else{
            rest += line;
            rest += "\n";
        }
        n++;
    }
    //TODO MAKE THIS BETTER!!!
    return {editedLine, rest};
}

// std::pair<std::string, std::string> excludeLines(const std::string& content, int lineNum)
// {
//     int n = 0;
//     std::istringstream iss(content);
//     std::string editedLine;
//     std::string rest;
//     for(std::string line; std::getline(iss, line);)
//     {
//         if(n == lineNum)
//         {
//             line = '`' + line + '`';
//         }
//         //TODO MAKE THIS BETTER
//         rest += line;
//         n++;
//     }
//     return {editedLine, rest};
// }

void debugTextEdit(CoolTextEdit* edit) {
    qDebug() << "=== QTextEdit Debug Info ===";
    qDebug() << "Plain text length:" << edit->toPlainText().length();
    qDebug() << "HTML length:" << edit->toHtml().length();
    qDebug() << "Is read only:" << edit->isReadOnly();
    qDebug() << "Has focus:" << edit->hasFocus();
    qDebug() << "Cursor position:" << edit->textCursor().position();
    qDebug() << "Selection:" << edit->textCursor().selectedText();
    qDebug() << "Document modified:" << edit->document()->isModified();
    qDebug() << "Plain text:" << edit->toPlainText();
    qDebug() << "HTML:" << edit->toHtml();
    qDebug() << "Document:" << edit->document();
    qDebug() << "Cursor position:" << edit->textCursor().position();
    qDebug() << "Old Content" << edit->oldContent;
    qDebug() << "New Content" << edit->newContent;
}


std::pair<int, int> CoolTextEdit::selectEditedText(std::string& content) {
    if (content == "") {
        return {0, content.length()};
    }
    QTextCursor cursor = this->textCursor();
    int pos = cursor.position();
    int start = pos -1;
    while (content[start] != '\n' && start != -1) {
        start--;
    }int end = pos;
    while (content[end] != '\n' && end != this->toPlainText().length()) {
        end++;
    }
    return {start + 1, end};
}

QString merge(std::string& rest, std::string& editedLine, int lineNum) {
    int n = 0;
    std::istringstream iss(rest);
    std::string result;
    for(std::string line; std::getline(iss, line);)
    {
        if(n == lineNum)
        {
            result += editedLine;
        }
        else{
            result += line;
        }
        n++;
    }
    if(n == lineNum)
    {
        result += editedLine;
    }
    return QString::fromStdString(result);
}



// void CoolTextEdit::refreshWidget(){
//     int lineNum = this->getLineNumber();
//     std::string content = this->toPlainText().toStdString();
//     std::pair<std::string, std::string> result = excludeLines(content, lineNum);
//     // std::string editedLine = result.first;
//     // std::string rest = result.second;
//     std::string oldTempContent = oldContent.toStdString();
//     std::pair<int, int> textCords = selectEditedText(content);
//     std::string editedLine = content.substr(textCords.first, textCords.second-textCords.first-1);
//     std::string before = content.substr(0, textCords.first);
//     std::string after = content.substr(textCords.second, content.length());
//     debugTextEdit(this);
//     const char* cstr = before.c_str();
//     QString html = QString(compile(cstr));
//     before = html.toStdString();
//     cstr = after.c_str();
//     html = QString(compile(cstr));
//     after = html.toStdString();
//     textCords = selectEditedText(content);
//     std::string newLine = content.substr(textCords.first, textCords.second-textCords.first-1);
//     oldContent = QString::fromStdString(before + newLine + after);
//     std::string merged = before + "<p>" + editedLine + "</p>" + after;
//     setHtml(QString::fromStdString(merged));
//
//
// }






int getHtmlLength(QString& htmlVal) {
    QTextDocument doc;
    doc.setHtml(htmlVal);
    QTextCursor cursor(&doc);
    cursor.movePosition(QTextCursor::End);
    int pos = cursor.position();
    if (pos == 0) {
        return 1;
    }
    return pos;
}

std::vector<TextBlock> extractTextBlocks(narrayInfo* narray) {
    std::vector<TextBlock> textBlocks;
    narrayInfo* nodes = narray->data[0]->children;
    int start, end = 0;
    for (int i = 0; i < nodes->elements; i++) {
        QString htmlVal = QString::fromStdString(std::string(to_html(nodes->data[i])));
        std::string mdVal = to_raw(nodes->data[i]);
        end = start + getHtmlLength(htmlVal);
        textBlocks.push_back(TextBlock{htmlVal, mdVal, start, end});
        start = end;
    }
    return textBlocks;

}


int getSelectedBlock(std::vector<TextBlock>& textBlocks, int pos) {
    for (int i = 0; i < textBlocks.size(); i++) {
        if (pos >= textBlocks[i].start && pos < textBlocks[i].end) {
            return i;
        }
    }
    return textBlocks.size();
}


std::string renderBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock) {
    std::string content;
    for (int i = 0; i < textBlocks.size(); i++) {
        if (i == selectedBlock) {
            content += textBlocks[i].mdVal;
        }
        else {
            content+= textBlocks[i].htmlVal.toStdString();
        }
    }
    return content;
}


std::string CoolTextEdit::refreshBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock) {
    std::string content;
    for (int i = 0; i < textBlocks.size(); i++) {
        if (i == selectedBlock) {
            content += this->toPlainText().toStdString().substr(textBlocks[i].start, textBlocks[i].end - textBlocks[i].start);
        }
        else {
            content+= textBlocks[i].mdVal;
        }
    }
    return content;
}

void CoolTextEdit::refreshWidget() {
    narrayInfo* narray;
    QTextCursor cursor = this->textCursor();
    int pos = cursor.position();
    if (textBlocks.size() == 0) {
        std::string text= this->toPlainText().toStdString();
        narray = compile_to_nodes(this->toPlainText().toStdString().c_str());
    }
    else {
        narray = compile_to_nodes(refreshBlocks(textBlocks, selectedBlock).c_str());

    }
    std::vector<TextBlock> textBlocks = extractTextBlocks(narray);
    int selectedBlock = getSelectedBlock(textBlocks, pos);
    std::string content = renderBlocks(textBlocks, selectedBlock);
    this->setHtml(QString::fromStdString(content));
    QTextCursor newCursor = this->textCursor();
    newCursor.setPosition(qMin(pos-1, this->document()->characterCount() - 1));
    this->setTextCursor(newCursor);
    this->textBlocks = textBlocks;
    this->pos = pos;
}
