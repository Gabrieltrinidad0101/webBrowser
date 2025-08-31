#include "ParserHtml.h"
#include <vector>

void ParserHtml::jumpSpace(const std::string &html, size_t &i)
{
    while (html[i] == ' ' || html[i] == '\n' || html[i] == '\t' || html[i] == '\r')
    {
        i++;
    }
}

std::vector<HtmlNode> ParserHtml::lexer(const std::string &html)
{
    std::vector<HtmlNode> tokens = {};
    for (size_t i = 0; i < html.size(); i++)
    {
        std::string tag = "";
        std::string innerText = "";
        std::map<std::string, std::string> attributes;
        this->jumpSpace(html, i);
        if (html[i] == '<')
        {
            i++;
            bool isOpen = i == '\\';
            if (isOpen)
            {
                i++;
            }
            while (html[i] != ' ' && html[i] != '>')
            {
                tag += html[i];
                i++;
            }
            this->jumpSpace(html, i);
            if (isOpen)
            {
                while (html[i] != '>')
                {
                    std::string key = "";
                    std::string value = "";
                    this->jumpSpace(html, i);
                    while (html[i] != ' ' && html[i] != '=')
                    {
                        key += html[i];
                        i++;
                    }
                    this->jumpSpace(html, i);
                    if (html[i] == '=')
                    {
                        this->jumpSpace(html, i);
                    }

                    while (html[i] != ' ' && html[i] != '>')
                    {
                        value += html[i];
                        i++;
                    }
                    attributes[key] = value;
                }
                i++;
                while (html[i] != '<')
                {
                    innerText += html[i];
                    i++;
                }
                HtmlNode node{
                    tag,
                    innerText,
                    attributes,
                    isOpen : true,
                };
                tokens.push_back(node);
                continue;
            }
            while (html[i] != '>')
            {
                i++;
            }
            HtmlNode node{
                tag,
                isOpen : false,
            };
            tokens.push_back(node);
        }
    }

    return tokens;
}

void ParserHtml::parser(const std::string &html)
{
    HtmlNode root;
    std::vector<HtmlNode> tokens = this->lexer(html);
    
    HtmlNode root;
    std::vector<HtmlNode> openTokens;
    for(HtmlNode token : tokens){
        if(token.isOpen){
            openTokens.push_back(token);
        }
    }
}