#include "Render/ParserHtml/ParserHtml.h"

int main()
{
    ParserHtml parser = ParserHtml();
    parser.parser("<div p=1 p2=23>Hello World <h1>dasdasdas</h1> </div>");
}
