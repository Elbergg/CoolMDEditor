#include "mainwindow.h"
#include "coolmd.h"
#include "./ui_mainwindow.h"

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

void MainWindow::on_textBrowser_intoHTML()
{
    QString qstr = ui->textEdit->toPlainText();
    QByteArray byteArray = qstr.toUtf8();
    char* cstr = byteArray.data();
    QString html = QString(compile(cstr));
    ui->textBrowser->setHtml(html);
}


void MainWindow::onTextChanged()
{
    QString qstr = ui->textEdit->toPlainText();
    QByteArray byteArray = qstr.toUtf8();
    char* cstr = byteArray.data();
    QString html = QString(compile(cstr));
    ui->textBrowser->setHtml(html);
}
