#include "CoolBlockBrowser.h"

#include "coolmd.h"
#include "parser.h"


CoolBlockBrowser::CoolBlockBrowser(QWidget* parent)
    : QWidget(parent), scrollArea(nullptr), container(nullptr), containerLayout(nullptr) {
    setupUi();
}



std::string join_blocks(std::vector<CoolTextEdit *> blocks) {
    std::string result;
    for (auto& block : blocks) {
        result += block->mdVal;
    }
    return result;
}

std::vector<CoolTextEdit*> CoolBlockBrowser::textBlocksFromNarray(narrayInfo* narray) {
    std::vector<CoolTextEdit*> textBlocks;
    narrayInfo* nodes = narray->data[0]->children;
    struct Node* bodyNode = (Node*)calloc(1, sizeof(struct Node));
    bodyNode->children  = createNodeArray(1);
    for (int i = 0; i < nodes->elements; i++) {
        addToNodeArray(bodyNode->children, nodes->data[i]);
        //TODO THE NEWLINE BLOCK CONSUMES THE HASHTAGS IN THE HEADER, BIG PROBLEM, HASHNODES DO NOT HAVE A VALUE
        QString htmlVal = QString::fromStdString(std::string(to_html(bodyNode)->data));
        std::string mdVal = to_raw(bodyNode)->data;
        // if (mdVal == "\n") {
        //     htmlVal = QString::fromStdString("<p>&nbsp;</p>");
        // }
        TextBlock tblock = TextBlock{htmlVal, mdVal};
        CoolTextEdit* cedit = new CoolTextEdit(this, htmlVal, mdVal);
        textBlocks.push_back(cedit);
        bodyNode->children->data[0] = NULL;
        bodyNode->children->elements = 0;
    }
    free_node(bodyNode);
    return textBlocks;

}


void CoolBlockBrowser::rerenderBlocks() {
    std::string text = join_blocks(textBlocks);
    narrayInfo* narray = compile_to_nodes(text.c_str());
    if (narray->data[0]->children->elements != 0) {
        textBlocks = textBlocksFromNarray(narray);
        clear();
        for (int i = 0; i < textBlocks.size(); i++) {
            insertWidget(i, textBlocks[i]);
        }
    }
    free_narray(narray);
}