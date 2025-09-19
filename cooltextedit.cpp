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
    qDebug() << edit->sourceText;
}


// std::pair<int, int> CoolTextEdit::selectEditedText() {
//     int lineNum = this->getLineNumber();
//     int i = 0;
//     std::string content = this->toPlainText().toStdString();
//
//
// }


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



void CoolTextEdit::refreshWidget(){
    int lineNum = this->getLineNumber();
    std::string content = this->toPlainText().toStdString();
    std::pair<std::string, std::string> result = excludeLines(content, lineNum);
    std::string editedLine = result.first;
    std::string rest = result.second;
    // std::pair<int, int> textCords = selectEditedText();
    // std::string editedLine = content.substr(textCords.first, textCords.second-textCords.first);
    debugTextEdit(this);
    // const QString qstr = this->toPlainText();
    // sourceText = qstr;
    // QByteArray byteArray = qstr.toUtf8();
    const char* cstr = rest.c_str();
    QString html = QString(compile(cstr));
    rest = html.toStdString();
    QString merged = merge(rest, editedLine, lineNum);
    setHtml(merged);


}
