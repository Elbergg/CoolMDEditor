#include "cooltextedit.h"
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


// std::pair<std::string, std::string> excludeLines(std::string content, int lineNum)
// {
//     int n = 0;
//     std::istringstream iss(content);
//     std::string editedLine;
//     std::string rest;
//     for(std::string line; std::getline(iss, line);)
//     {
//         if(n == lineNum)
//         {
//             editedLine = line;
//         }
//         else{
//             rest += line;
//         }
//         n++;
//     }
//     return {editedLine, rest};
// }

std::pair<std::string, std::string> excludeLines(std::string content, int lineNum)
{
    int n = 0;
    std::istringstream iss(content);
    std::string editedLine;
    std::string rest;
    for(std::string line; std::getline(iss, line);)
    {
        if(n == lineNum)
        {
            line = '`' + line + '`';
        }
        //TODO MAKE THIS BETTER
        rest += line;
        n++;
    }
    return {editedLine, rest};
}
void CoolTextEdit::refreshWidget(){
    int lineNum = this->getLineNumber();
    std::string content = this->toPlainText().toStdString();
    std::pair<std::string, std::string> result = excludeLines(content, lineNum);
    std::string editedLine = result.first;
    std::string rest = result.second;


}
