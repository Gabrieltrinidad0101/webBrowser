#include <string>
#include <vector>
#include <map>

#ifndef PARSER_HTML_H
#define PARSER_HTML_H

struct HtmlNode {
    std::string tag;  
    std::string innerText;                     
    std::map<std::string, std::string> attributes;
    std::vector<HtmlNode> children;             
    bool isOpen;  
};

class ParserHtml {
public:
    void parser(const std::string& html);

private:
    std::vector<HtmlNode> lexer(const std::string& html);
    void jumpSpace(const std::string& html,size_t& i);
};

#endif // PARSER_HTML_H
