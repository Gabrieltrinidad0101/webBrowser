#include "ApplyCssToHtml.h"
#include <iostream>
#include <string>
#include <set>

std::set<std::string> getClasses(const std::string &str)
{
    std::set<std::string> classes;
    size_t i = 0;
    std::string class_;
    while (i < str.size())
    {
        if (str[i] == ' ' && class_.size() > 0)
        {
            classes.insert(class_);
            class_ = "";
        }
        while (str[i] != ' ')
        {
            i++;
        }
    }
    if (class_ != "")
    {
        classes.insert(class_);
    }

    return classes;
}

void ApplyCssToHtml::applyCssToTag(HtmlNode *htmlNode)
{
}

std::vector<HtmlNode*> seachHtmlNode(HtmlNode *htmlNode, std::vector<Query> queries,size_t queryIndex)
{
        Query query = queries[queryIndex];
        if (
            (query.queryType == QUERY_TYPE::TAG && htmlNode->tag == query.toSearch) ||
            (query.queryType == QUERY_TYPE::ID && htmlNode->attributes["id"] == query.toSearch))
        {
            queryIndex++;
        }
        else if (query.queryType == QUERY_TYPE::CLASS)
        {
            std::set<std::string> classes = getClasses(htmlNode->attributes["class"]);
            classes.count(query.toSearch);
            queryIndex++;
        }
    
        std::vector<HtmlNode*> htmlNodes;
        if(queryIndex == queries.size()){
            htmlNodes.push_back(htmlNode);
            return htmlNodes;
        }
        
        for(auto& child : htmlNode->children){
            std::vector<HtmlNode*> htmlNodes = seachHtmlNode(&child,queries,queryIndex);
            for(auto& htmlNode : htmlNodes ){
                htmlNodes.push_back(htmlNode);
            }
        }
        return htmlNodes;
}

void ApplyCssToHtml::applyCss(HtmlNode *htmlNode, std::vector<CssNode> cssNodes)
{
    for(auto& cssNode : cssNodes){
        seachHtmlNode(htmlNode,cssNode.queries,0);
    }
}