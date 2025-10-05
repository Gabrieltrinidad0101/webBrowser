#ifndef APPLY_CSS_TO_HTML_H
#define APPLY_CSS_TO_HTML_H
#include "ParserHtml/ParserHtml.h"
#include "ParserCss/ParserCss.h"

class ApplyCssToHtml
{
private:
    void applyCssToTag(HtmlNode *htmlNode);

public:
    void applyCss(HtmlNode &htmlNode, std::vector<CssNode> cssNodes);
    void applyAndAnalyze(ComponentUI componentUI, std::map<std::string, std::string> attributes);
    ComponentUI &ApplyCssToHtml::htmlNodeToComponentUI(HtmlNode &htmlNode, CssNode &cssNode, size_t queryIndex)
};

#endif // APPLY_CSS_TO_HTML_H