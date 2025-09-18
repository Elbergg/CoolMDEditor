#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "coolmd.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged()
{
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    // QString html = ui->textEdit->toPlainText();
    // ui->textEdit->setHtml(html);
    QString qstr = ui->textEdit->toPlainText();
    QByteArray byteArray = qstr.toUtf8();
    char* cstr = byteArray.data();
    QString html = QString(compile(cstr));
    ui->textEdit->setHtml(html);
    QString tempText = ui->textEdit->document()->toRawText();
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
}



// void MainWindow::onTextChanged()
// {
//     ui->textEdit->setHtml(ui->textEdit->toHtml());
    // QString qstr = ui->textEdit->toPlainText();
    // QByteArray byteArray = qstr.toUtf8();
    // char* cstr = byteArray.data();
    // QString html = QString(compile(cstr));
    // ui->textBrowser->setHtml(html);
// }
