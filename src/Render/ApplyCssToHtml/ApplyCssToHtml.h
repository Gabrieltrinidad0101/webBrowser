#ifndef APPLY_CSS_TO_HTML_H
#define APPLY_CSS_TO_HTML_H
#include "ParserHtml/ParserHtml.h"
#include "ParserCss/ParserCss.h"
#include "ComponentUI/ComponentUI.h"

class ApplyCssToHtml
{
private:
    void applyCssToTag(HtmlNode *htmlNode);

public:
    ComponentUI* htmlNodeToComponentUI(HtmlNode* htmlNode,CssNode &cssNode, size_t queryIndex,std::vector<ComponentUI*>& componentUIs);
    std::vector<ComponentUI *> applyCss(HtmlNode htmlNode, std::vector<CssNode> cssNodes); 
    void applyAndAnalyze(ComponentUI *componentUI, std::map<std::string, std::string> attributes);
};

#endif // APPLY_CSS_TO_HTML_H