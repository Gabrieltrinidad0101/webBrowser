#include <vector>
#include <string>
#include "Color/Color.h"
#include "ParserCss/ParserCss.h"
#ifndef COMPONENT_UI_H
#define COMPONENT_UI_H


class ComponentUI
{
public:

    CssStyle cssStyle;
    std::vector<ComponentUI*> children;
    size_t childrenWidth = 0;

    ComponentUI(){
        
    };

    ComponentUI& setX(int x) { this->cssStyle.x = x; return *this; }
    ComponentUI& setY(int y) { this->cssStyle.y = y; return *this; }
    ComponentUI& setGAP(uint gap) { this->cssStyle.gap = gap; return *this; }
    ComponentUI& setRenderW(int w) { this->cssStyle.w = w; return *this; }
    ComponentUI& setRenderH(int h) { this->cssStyle.h = h; return *this; }
    ComponentUI& setDisplay(DISPLAY d) { this->cssStyle.display = d; return *this; }
    ComponentUI& setJustifyContent(JUSTIFY_CONTENT jc) { this->cssStyle.justifyContent = jc; return *this; }
    ComponentUI& setAlignItems(ALIGN_ITEMS ai) { this->cssStyle.alignItems = ai; return *this; }
    ComponentUI& setBgColor(const std::string& c) { this->cssStyle.bgColor = Color(c); return *this; }
    ComponentUI& setChildren(const std::vector<ComponentUI*> c) { this->children = c; return *this; }
    ComponentUI& setBorderColor(const std::string& c) { this->cssStyle.borderColor = Color(c); return *this; }
    void build();

private:
    void position();
    void flexJustifyContent();
    void flexAlignItems();
    void defaultPosition();
};

#endif // COMPONENT_UI_H
