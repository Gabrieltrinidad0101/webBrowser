#include <vector>
#include <string>
#include "Color/Color.h"

#ifndef COMPONENT_UI_H
#define COMPONENT_UI_H

enum DISPLAY
{
    FLEX,
    GRID,
    INLINE,
    BLOCK
};

enum class JUSTIFY_CONTENT
{
    CENTER,
    SPACE_BETWEEN,
    SPACE_EVENLY,
    SPACE_AROUND
};

enum class ALIGN_ITEMS
{
    STRETCH,
    CENTER,
    FLEX_START,
    FLEX_END
};

class ComponentUI
{
public:

    int yWithBorder;
    int xWithBorder;
    int wWithBorder;
    int hWithBorder;
    int x;
    int y;
    int w = 0 ;
    int h = 0;

    CssStyle cssStyle;

    uint GAP = 0;
    RGBA bgColor;
    RGBA borderColor;
    RGBA borderLeftColor;
    RGBA borderRightColor;
    RGBA borderTopColor;
    RGBA borderBottomColor;

    uint borderLeftWidth = 0;
    uint borderRightWidth = 0;
    uint borderTopWidth = 0;
    uint borderBottomWidth = 0;
    int childrenWidth = 0;

    DISPLAY display;
    JUSTIFY_CONTENT justifyContent;
    ALIGN_ITEMS alignItems;

    std::vector<ComponentUI*> children;

    ComponentUI(){
        
    };

    ComponentUI& setX(int x) { this->x = x; return *this; }
    ComponentUI& setY(int y) { this->y = y; return *this; }
    ComponentUI& setGAP(uint gap) { this->GAP = gap; return *this; }
    ComponentUI& setRenderW(int w) { this->w = w; return *this; }
    ComponentUI& setRenderH(int h) { this->h = h; return *this; }
    ComponentUI& setDisplay(DISPLAY d) { this->display = d; return *this; }
    ComponentUI& setJustifyContent(JUSTIFY_CONTENT jc) { this->justifyContent = jc; return *this; }
    ComponentUI& setAlignItems(ALIGN_ITEMS ai) { this->alignItems = ai; return *this; }
    ComponentUI& setBgColor(const std::string& c) { this->bgColor = Color(c); return *this; }
    ComponentUI& setChildren(const std::vector<ComponentUI*>& c) { this->children = c; return *this; }
    ComponentUI& setBorderColor(const std::string& c) { this->borderColor = Color(c); return *this; }
    void build();

private:
    void position();
    void flexJustifyContent();
    void flexAlignItems();
    void defaultPosition();
};

#endif // COMPONENT_UI_H
