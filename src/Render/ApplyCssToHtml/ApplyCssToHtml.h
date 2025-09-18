#ifndef APPLY_CSS_TO_HTML_H
#define APPLY_CSS_TO_HTML_H
#include "ParserHtml/ParserHtml.h"
#include "ParserCss/ParserCss.h"

class ApplyCssToHtml
{
private:
    void ApplyCssToHtml::applyCssToTag(HtmlNode *htmlNode);

public:
    void applyCss(HtmlNode *htmlNode, std::vector<CssNode> cssNodes);
};

#endif // APPLY_CSS_TO_HTML_H