#include "ComponentUI.h"
#include <iostream>
#include "../Utils/print.h"

void ComponentUI::flexJustifyContent()
{
    if (this->cssStyle.justifyContent == JUSTIFY_CONTENT::CENTER)
    {
        int x = this->cssStyle.x + (this->cssStyle.wWithBorder - this->childrenWidth) / 2;
        for (auto &child : this->children)
        {
            child->cssStyle.x = x;
            child->cssStyle.y = this->cssStyle.y;  
            x += child->cssStyle.wWithBorder + this->cssStyle.gap;
        }
        return;
    }

    if (this->cssStyle.justifyContent == JUSTIFY_CONTENT::SPACE_BETWEEN)
    {
        int increase = 0;
        if(this->children.size() > 1){
            increase = (this->cssStyle.wWithBorder - this->childrenWidth) / (this->children.size() - 1);
        }
        int x = this->cssStyle.x;
        for (auto &child : this->children)
        {
            child->cssStyle.x = x;
            child->cssStyle.y = this->cssStyle.y;  
            x += child->cssStyle.wWithBorder + increase;
        }
        return;
    }

    if (this->cssStyle.justifyContent == JUSTIFY_CONTENT::SPACE_EVENLY){
        int increase = (this->cssStyle.wWithBorder - this->childrenWidth) / (this->children.size() + 1);
        int x = this->cssStyle.x + increase;
        for (auto &child : this->children)
        {
            child->cssStyle.x = x;
            child->cssStyle.y = this->cssStyle.y;  
            x += child->cssStyle.wWithBorder + increase;
        }
        return;
    }

}

void ComponentUI::flexAlignItems(){
    if (this->cssStyle.alignItems == ALIGN_ITEMS::CENTER){
        for (auto &child : this->children){
            child->cssStyle.y = (this->cssStyle.width - child->cssStyle.wWithBorder)/2; 
        }
        return;
    }

    if(this->cssStyle.alignItems == ALIGN_ITEMS::FLEX_START){
        for (auto &child : this->children){
            child->cssStyle.y = this->cssStyle.height;
        }
        return;
    }

    if(this->cssStyle.alignItems == ALIGN_ITEMS::FLEX_END){
        for (auto &child : this->children){
            child->cssStyle.y = this->cssStyle.y + this->cssStyle.height - child->cssStyle.hWithBorder;
        }
        return;
    }

}


void ComponentUI::defaultPosition(){
    int x = this->cssStyle.x;
    int y = this->cssStyle.y;
    for(auto &child : this->children){
        child->cssStyle.x = x;
        child->cssStyle.y = y;
        if(child->cssStyle.display == DISPLAY::BLOCK){
            y += child->cssStyle.hWithBorder;
        }else if (child->cssStyle.display == DISPLAY::INLINE){
            x += child->cssStyle.wWithBorder;
        }
    }
}


void ComponentUI::position()
{
    if (this->cssStyle.display == DISPLAY::FLEX){
        this->flexJustifyContent();
        this->flexAlignItems();
        return;
    }

    this->defaultPosition();
}



void ComponentUI::build()
{
    for (ComponentUI* child : this->children){
        this->childrenWidth += child->cssStyle.wWithBorder;
    }

    this->cssStyle.wWithBorder = this->cssStyle.width + this->cssStyle.borderLeftWidth + this->cssStyle.borderRightWidth;
    this->cssStyle.hWithBorder = this->cssStyle.height + this->cssStyle.borderTopWidth + this->cssStyle.borderBottomWidth;
    this->cssStyle.xWithBorder = this->cssStyle.x - this->cssStyle.borderLeftWidth;
    this->cssStyle.yWithBorder = this->cssStyle.y - this->cssStyle.borderLeftWidth;

    this->position();
}