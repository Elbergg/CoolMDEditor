#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "coolmd.h"
#include <QFileDialog>
#include <QMessageBox>
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
    int pos = ui->textEdit->textCursor().position();
    ui->textEdit->pos = pos;
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->pos);
    ui->textEdit->setTextCursor(cursor);
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    ui->textEdit->refreshWidget();
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

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "",
                                                    tr("All Files (*);;Text Files (*.txt)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            ui->textEdit->setText(content);  // ← Put content in QTextEdit
            file.close();
        } else {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Could not open file"));
        }
    }
}

