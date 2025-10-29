//
// Created by tomek on 10/27/25.
//
# pragma once
#ifndef COOLBLOCKBROWSER_H
#define COOLBLOCKBROWSER_H
#include <QScrollArea>
#include <QVBoxLayout>
#include "cooltextedit.h"


class CoolBlockBrowser : public QWidget{

public:

    CoolBlockBrowser(QWidget* parent = nullptr);
    void addWidget(CoolTextEdit *widget) {
        containerLayout->addWidget(widget);
    }
    void insertWidget(int index, QWidget *widget) {
        containerLayout->insertWidget(index, widget);
    }

    void removeWidget(CoolTextEdit *widget) {
        containerLayout->removeWidget(widget);
        widget->deleteLater();
    }

    void clear() {
        while (containerLayout->count() > 0) {
            QLayoutItem *item = containerLayout->takeAt(0);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
    }


    void rerenderBlocks();
private:
    QScrollArea *scrollArea;
    QWidget *container;
    QVBoxLayout *containerLayout;
    std::vector<TextBlock> textBlocks;

    void setupUi() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        container = new QWidget();
        containerLayout = new QVBoxLayout(container);
        containerLayout->setAlignment(Qt::AlignTop);
        containerLayout->setSpacing(5);
        containerLayout->setContentsMargins(10, 10, 10, 10);
        scrollArea->setWidget(container);
        mainLayout->addWidget(scrollArea);
        CoolTextEdit* cedit = new CoolTextEdit();
        insertWidget(0, cedit);
    }
};




#endif //COOLBLOCKBROWSER_H
