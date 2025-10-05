#include "cooltextedit.h"
#include "coolmd.h"
#include <QTextEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QLabel>
#include <string>
#include <sstream>
#include "algorithm"
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
    // qDebug() << "Old Content" << oldContent;
    // qDebug() << "New Content" << newContent;
}





// int getHtmlLength(QString& htmlVal) {
//     QTextDocument doc;
//     doc.setHtml(htmlVal);
//     QTextCursor cursor(&doc);
//     cursor.movePosition(QTextCursor::End);
//     int pos = cursor.position();
//     if (pos == 0) {
//         return 1;
//     }
//     return pos;
// }

int replace(std::string& str, const std::string& from, const std::string& to, int& start) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return -1;
    str.replace(start_pos, from.length(), to);
    return start_pos + from.length();
}

void CoolTextEdit::highlightText(int begin, int end) {
    QTextCursor clearCursor(this->document());
    clearCursor.select(QTextCursor::Document);
    QTextCharFormat clearFmt;
    clearFmt.setBackground(Qt::transparent);
    clearCursor.mergeCharFormat(clearFmt);
    QTextCharFormat fmt;
    fmt.setBackground(Qt::yellow);
    QTextCursor cursor(this->document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(fmt);
}


void assignHtmlLength(std::vector<TextBlock>& textBlocks, int selected_block) {
    QTextDocument doc;
    QTextCursor cursor(&doc);
    std::string html;
    int start = 1;
    for (int i = 0; i < textBlocks.size(); ++i) {
        textBlocks[i].start = start;
        if (i == selected_block) {
            int j = 0;
            std::string val = textBlocks[i].mdVal;
            while (replace(val, "\n", "<br>", j)!= -1);
            html +=  val;
        }
        else {
            html += textBlocks[i].htmlVal.toStdString();
        }
        doc.setHtml(QString::fromStdString(html));
        cursor.movePosition(QTextCursor::End);
        if (start == cursor.position()+1) {
            textBlocks[i].end = start-1;
            textBlocks[i].start = start -1;
            start = cursor.position()+1;
        }else {
            textBlocks[i].end = cursor.position() + 1;
            start = cursor.position() + 2;
        }


    }
}
std::vector<TextBlock> extractTextBlocks(narrayInfo* narray, int selected_block) {
    std::vector<TextBlock> textBlocks;
    narrayInfo* nodes = narray->data[0]->children;
    for (int i = 0; i < nodes->elements; i++) {
        //TODO THE NEWLINE BLOCK CONSUMES THE HASHTAGS IN THE HEADER, BIG PROBLEM, HASHNODES DO NOT HAVE A VALUE
        QString htmlVal = QString::fromStdString(std::string(to_html(nodes->data[i])));
        std::string mdVal = to_raw(nodes->data[i]);
        textBlocks.push_back(TextBlock{htmlVal, mdVal});
    }
    assignHtmlLength(textBlocks, selected_block);
    return textBlocks;

}


// std::vector<TextBlock> extractTextBlocks(narrayInfo* narray) {
//     std::vector<TextBlock> textBlocks;
//     narrayInfo* nodes = narray->data[0]->children;
//     int start, end = 0;
//     for (int i = 0; i < nodes->elements; i++) {
//         QString htmlVal = QString::fromStdString(std::string(to_html(nodes->data[i])));
//         std::string mdVal = to_raw(nodes->data[i]);
//         end = start + getHtmlLength(htmlVal);
//         textBlocks.push_back(TextBlock{htmlVal, mdVal, start, end});
//         start = end;
//     }
//     return textBlocks;
//
// }

int getSelectedBlock(std::vector<TextBlock>& textBlocks, int pos) {
    for (int i = 0; i < textBlocks.size(); i++) {
        if (pos >= textBlocks[i].start && pos <= textBlocks[i].end) {
            return i;
        }
    }
    return textBlocks.size();
}





std::string CoolTextEdit::renderBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock) {
    std::string content;
    for (int i = 0; i < textBlocks.size(); i++) {
        if (i == selectedBlock) {
            std::string val = textBlocks[i].mdVal;
            int j = 0;
            //TODO fix the problem where a paragraph adds a newline with every eddit
            while (replace(val, "\n", "<br>", j)!= -1);
            content +=  val;

        }
        else {
            content+= textBlocks[i].htmlVal.toStdString();
        }
    }
    assignHtmlLength(textBlocks, selectedBlock);
    return content;
}


std::string simulateToPlainText(const std::string& input) {
    QTextDocument doc;
    doc.setHtml(QString::fromStdString(input));
    return doc.toPlainText().toStdString();
}

std::string CoolTextEdit::refreshBlocks(std::vector<TextBlock>& textBlocks, int selectedBlock) {
    std::string content;
    for (int i = 0; i < textBlocks.size(); i++) {
        if (i == selectedBlock) {
            std::string changedText = this->toPlainText().toStdString().substr(textBlocks[i].start-1, textBlocks[i].end - textBlocks[i].start + getTextDiffLen() + 1);
            textBlocks[i].mdVal = changedText;
            content += changedText;
        }
        else {
            content+= textBlocks[i].mdVal;
        }
    }
    return content;
}


int CoolTextEdit::getTextDiffLen() {
    return newContent.length() - oldContent.length();
}

void CoolTextEdit::refreshWidget() {
    newContent = this->toPlainText().toStdString();
    narrayInfo* narray;
    QTextCursor cursor = this->textCursor();
    int pos = cursor.position();
    // int selectedBlock = getSelectedBlock(textBlocks, pos);
    if (this->textBlocks.size() == 0) {
        std::string text= this->toPlainText().toStdString();
        narray = compile_to_nodes(this->toPlainText().toStdString().c_str());
    }
    else {
        narray = compile_to_nodes(refreshBlocks(textBlocks, selectedBlock).c_str());
    }
    std::vector<TextBlock> textBlocks = extractTextBlocks(narray, this->selectedBlock);
    int selectedBlock = getSelectedBlock(textBlocks, pos);
    std::string content = renderBlocks(textBlocks, selectedBlock);
    this->setHtml(QString::fromStdString(content));
    QTextCursor newCursor = this->textCursor();
    newCursor.setPosition(qMin(pos, this->document()->characterCount() - 1));
    this->setTextCursor(newCursor);
    this->textBlocks = textBlocks;
    this->pos = pos;
    this->selectedBlock = selectedBlock;
    oldContent = this->toPlainText().toStdString();
    highlightText(textBlocks[selectedBlock].start-1, textBlocks[selectedBlock].end);
}
