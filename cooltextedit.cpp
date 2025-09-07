#include "cooltextedit.h"
#include <QTextEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QLabel>
#include <string>
#include <sstream>
CoolTextEdit::CoolTextEdit() {}

int CoolTextEdit::getLineNumber(){
    QTextCursor cursor = this->textCursor();
    int position = cursor.position();
    QTextDocument *doc = this->document();
    QTextBlock block = doc->findBlock(position);
    return block.blockNumber();
}

void CoolTextEdit::refreshWidget(){
    int lineNum = this->getLineNumber();
    std::string content = this->toPlainText().toStdString();
    std::istringstream iss(content);
    int n = 0;
    std::string editedLine;
    std::string rest;
    for(std::string line; std::getline(iss, line);)
    {
        if(n == lineNum)
        {
            editedLine = line;
        }
        else{
            rest += line;
        }
        n++;
    }

}
