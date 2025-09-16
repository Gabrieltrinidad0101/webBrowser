#include "Render/ParserHtml/ParserHtml.h"
#include "Render/ParserCss/ParserCss.h"

int main()
{
    ParserHtml parser = ParserHtml();
    ParserCss parserCss = ParserCss();
    std::pair<HtmlNode*, std::vector<std::string>> values = parser.parser("<div p=1 p2=23>Hello World <h1>dasdasdas</h1> </div>");
    
    parserCss.parser(&values.second);
    return 0;
}
