#include "Render/ParserHtml/ParserHtml.h"
#include "Render/ParserCss/ParserCss.h"

int main()
{
    ParserHtml parser = ParserHtml();
    ParserCss parserCss = ParserCss();
    std::pair<HtmlNode*, std::vector<std::string>> values = parser.parser(R"(
        <div class="container">
            <div class="child1"><div>
            <div class="child2"><div>
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
    parserCss.parser(values.second);
    return 0;
}
