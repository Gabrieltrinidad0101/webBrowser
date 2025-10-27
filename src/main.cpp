#include "Render/ParserHtml/ParserHtml.h"
#include "Render/ParserCss/ParserCss.h"
#include <iostream>
#include <string>
#include "ApplyCssToHtml/ApplyCssToHtml.h"
#include "UI/Ui.h"
#include "Utils/print.h"
int main()
{
    ParserHtml parser = ParserHtml();
    ParserCss parserCss = ParserCss();
    std::pair<HtmlNode*, std::string> values = parser.parser(R"(
        <div class="container">
            <div class="child1">
            </div>
            <div class="child2">
            </div>
        </div>

        <style>
            .container{
                display: flex;
                align-item: center;
                justify-content: space-between;
                background-color: red;
                width: 200px;
                height: 200px;
            }

            .child1 {
                background-color: blue;
                width: 50px;
                height: 50px;
            }

            .child2 {
                background-color: yellow;
                width: 50px;
                height: 50px;
            }
        <style/>
    )");
    std::vector<CssNode> cssesNode = parserCss.parser(values.second);
    ApplyCssToHtml applyCssToHtml = ApplyCssToHtml();
    std::vector<ComponentUI *> componentUIs = applyCssToHtml.applyCss(values.first, cssesNode);
    initUI(componentUIs);
    return 0;
}
