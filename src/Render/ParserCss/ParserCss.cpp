#include "ParserCss.h"
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>

void ParserCss::jumpSpace(const std::string &css, size_t &i)
{
    while (i < css.size() && (css[i] == ' ' || css[i] == '\n' || css[i] == '\t' || css[i] == '\r'))
        i++;
}

std::vector<CssLexer> ParserCss::lexer(const std::string &css)
{
    std::vector<CssLexer> tokens;
    size_t i = 0;
    while (i < css.size())
    {
        jumpSpace(css, i);
        std::string query;
        if (css[i] == '.')
        {
            query += css[i];
            i++;
        }
        jumpSpace(css, i);
        while (css[i] != ' ' && css[i] != '{')
        {
            query += css[i];
            i++;
        }
        jumpSpace(css, i);
        if (css[i] == '{')
        {
            i++;
        }
        jumpSpace(css, i);
        std::map<std::string, std::string> attributes;

        while (css[i] != '}')
        {
            std::string key, value;
            while (css[i] != ':')
            {
                key += css[i];
                i++;
            }
            if (css[i] == ':')
            {
                i++;
            }
            jumpSpace(css, i);
            while (css[i] != ';')
            {
                value += css[i];
                i++;
            }
            attributes[key] = value;
            jumpSpace(css, i);
        }

        CssLexer cssNode{
            query,
            attributes};

        tokens.push_back(cssNode);
    }
}

std::vector<Query> ParserCss::queryParser(const std::string &queriesString)
{
    std::vector<Query> queries;
    size_t i = 0;
    while (i < queriesString.size())
    {
        jumpSpace(queriesString, i);
        QUERY_TYPE queryType = TAG;
        std::string toSearch;
        if (i == '.')
        {
            queryType = CLASS;
        }
        else if (i == '#')
        {
            queryType = ID;
        }
        if (queryType != TAG)
        {
            i++;
        }

        while (queriesString[i] != ' ')
        {
            toSearch += queriesString[i];
            i++;
        }
        jumpSpace(queriesString, i);
        QUERY_OPERATION queryOperation = ALL_CHILDREN;
        if (queriesString[i] == '>')
        {
            queryOperation = ONLY_CHILDREN;
        }

        Query query{
            queryType,
            toSearch,
            queryOperation};

        queries.push_back(query);
    }
    return queries;
}

size_t removeEnd(std::string s, std::string toReplace)
{
    if (s.size() > toReplace.size() && s.substr(s.size() - toReplace.size()) == toReplace)
        s = s.substr(0, s.size() - toReplace.size());
    return std::stoul(s);
}

std::vector<CssNode> ParserCss::parser(const std::string &css)
{
    std::vector<CssLexer> cssesLexer = lexer(css);
    std::vector<CssNode> cssesNode;
    for (CssLexer cssLexer : cssesLexer)
    {
        CssNode cssNode;
        cssNode.queries = queryParser(cssLexer.query);
        for (const auto &attribute : cssLexer.attributes)
        {
            if (attribute.first == "width")
            {
                cssNode.width = removeEnd(attribute.second, "px");
            }
            if (attribute.first == "height")
            {
                cssNode.height = removeEnd(attribute.second, "px");
            }

            auto it = displayMap.find(attribute.first);
            if (it != displayMap.end())
                cssNode.display = it->second;    
        }
        cssesNode.push_back(cssNode);
    }
    return cssesNode;
}
