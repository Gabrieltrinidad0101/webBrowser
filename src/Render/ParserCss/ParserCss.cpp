#include "ParserCss.h"
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <any>
#include "../Utils/print.h"

void ParserCss::jumpSpace(const std::string &css, size_t &i)
{
    while (i < css.size() && (css[i] == ' ' || static_cast<int>(css[i]) == 10 || css[i] == '\t' || css[i] == '\r')){
        i++;

    }
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

        while (i < css.size() && css[i] != '}')
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
            i++;
            attributes[key] = value;
            jumpSpace(css, i);
        }

        if(css[i] == '}'){
            i++;
        }
        jumpSpace(css, i);

        CssLexer cssNode{
            query,
            attributes};

        tokens.push_back(cssNode);
    }
    return tokens;
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
        print(queriesString[i]);
        if (queriesString[i] == '.')
        {
            queryType = CLASS;
        }
        else if (queriesString[i] == '#')
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


std::vector<CssNode> ParserCss::parser(std::string css)
{
    std::vector<CssNode> cssesNode;
    std::vector<CssLexer> cssesLexer = lexer(css);
    for (CssLexer cssLexer : cssesLexer)
    {
        CssNode cssNode;
        cssNode.queries = queryParser(cssLexer.query);
        cssNode.attributes = cssLexer.attributes;
        cssesNode.push_back(cssNode);
    }
    return cssesNode;
}
