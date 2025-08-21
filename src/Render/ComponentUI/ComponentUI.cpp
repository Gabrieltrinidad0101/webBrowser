#include "ComponentUI.h"
#include <iostream>

void ComponentUI::flexJustifyContent()
{
    if (this->justifyContent == JUSTIFY_CONTENT::CENTER)
    {
        int x = this->x + (this->wWithBorder - this->childrenWidth) / 2;
        for (auto &child : this->children)
        {
            child->x = x;
            child->y = this->y;  
            x += child->wWithBorder + this->GAP;
        }
        return;
    }

    if (this->justifyContent == JUSTIFY_CONTENT::SPACE_BETWEEN)
    {
        int increase = (this->wWithBorder - this->childrenWidth) / (this->children.size() - 1);
        int x = this->x;
        for (auto &child : this->children)
        {
            child->x = x;
            child->y = this->y;  
            x += child->wWithBorder + increase;
        }
        return;
    }

    if (this->justifyContent == JUSTIFY_CONTENT::SPACE_EVENLY){
        int increase = (this->wWithBorder - this->childrenWidth) / (this->children.size() + 1);
        int x = this->x + increase;
        for (auto &child : this->children)
        {
            child->x = x;
            child->y = this->y;  
            x += child->wWithBorder + increase;
        }
        return;
    }

}

void ComponentUI::flexAlignItems(){
    if (this->alignItems == ALIGN_ITEMS::CENTER){
        for (auto &child : this->children){
            child->y = (this->w - child->wWithBorder)/2; 
        }
        return;
    }

    if(this->alignItems == ALIGN_ITEMS::FLEX_START){
        for (auto &child : this->children){
            child->y = this->h;
        }
        return;
    }

    if(this->alignItems == ALIGN_ITEMS::FLEX_END){
        for (auto &child : this->children){
            child->y = this->y + this->h - child->hWithBorder;
        }
        return;
    }

}


void ComponentUI::defaultPosition(){
    int x = this->x;
    int y = this->y;
    for(auto &child : this->children){
        child->x = x;
        child->y = y;
        if(child->display == DISPLAY::BLOCK){
            y += child->hWithBorder;
        }else if (child->display == DISPLAY::INLINE){
            x += child->wWithBorder;
        }
    }
}


void ComponentUI::position()
{
    if (this->display == DISPLAY::FLEX){
        this->flexJustifyContent();
        this->flexAlignItems();
        return;
    }

    this->defaultPosition();
}



void ComponentUI::build()
{
    for (auto &child : this->children){
        this->childrenWidth += child->wWithBorder;
    }

    this->wWithBorder = this->w + this->borderLeftWidth + this->borderRightWidth;
    this->hWithBorder = this->h + this->borderTopWidth + this->borderBottomWidth;
    this->xWithBorder = this->x - this->borderLeftWidth;
    this->yWithBorder = this->y - this->borderLeftWidth;

    this->position();
}