//
// Created by tomek on 10/27/25.
//

#include "CoolBlockBrowser.h"


CoolBlockBrowser::CoolBlockBrowser(QWidget* parent)
    : QWidget(parent), scrollArea(nullptr), container(nullptr), containerLayout(nullptr) {
    setupUi();
}

void CoolBlockBrowser::rerenderBlocks() {  }

