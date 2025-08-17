#include "ComponentUI.h"
#include <iostream>

void ComponentUI::flex()
{
    if (this->justifyContent == JUSTIFY_CONTENT::CENTER)
    {
        int x = this->x + (this->w - this->childrenWidth) / 2;
        for (auto &child : this->children)
        {
            child->x = x;
            x += child->w + this->GAP;
        }
        return;
    }
    if (this->justifyContent == JUSTIFY_CONTENT::SPACE_BETWEEN)
    {
        int increase = (this->w - this->childrenWidth) / (this->children.size() - 1);
        int x = this->x;
        for (auto &child : this->children)
        {
            child->x = x;
            child->y = this->y;  
            std::cout << "x " << child->x << std::endl;
            x += child->w + increase;
        }
    }
}

void ComponentUI::position()
{
    if (this->display == DISPLAY::FLEX)
        this->flex();
}



void ComponentUI::build()
{
    for (auto &child : this->children){
        this->childrenWidth += child->w;
    }

    this->w = this->renderW + this->borderLeftWidth + this->borderRightWidth;
    this->h = this->renderH + this->borderTopWidth + this->borderBottomWidth;

    this->position();
}