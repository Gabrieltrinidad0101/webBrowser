#include "ApplyCssToHtml.h"
#include "Render/ComponentUI/ComponentUI.h"
#include <iostream>
#include <string>
#include <set>

std::set<std::string> getClasses(const std::string &str)
{
    std::set<std::string> classes;
    int i = 0;
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
    ComponentUI component;
    if (htmlNode->tag == "div")
    {
        component.cssStyle = CssStyle{
            color : {0, 0, 0, 1},
        };
    }
}

std::vector<HtmlNode*> seachHtmlNode(HtmlNode *htmlNode, std::vector<Query> queries,size_t index)
{
    auto query = queries[index];
    if (
        (query.queryType == QUERY_TYPE::TAG && htmlNode->tag == query.toSearch) ||
        (query.queryType == QUERY_TYPE::ID && htmlNode->attributes["id"] == query.toSearch))
    {
        index++;
    }
    else if (query.queryType == QUERY_TYPE::CLASS)
    {
        std::set<std::string> classes = getClasses(htmlNode->attributes["class"]);
        classes.count(query.toSearch);
        index++;
    }

    std::vector<HtmlNode*> htmlNodes;
    if(index == queries.size()){
        htmlNodes.push_back(htmlNode);
        return htmlNodes;
    }
    
    for(auto& child : htmlNode->children){
        std::vector<HtmlNode*> htmlNodes = seachHtmlNode(&child,queries,index);
        for(auto& htmlNode : htmlNodes ){
            htmlNodes.push_back(htmlNode);
        }
    }
}

void ApplyCssToHtml::applyCss(HtmlNode *htmlNode, std::vector<CssNode> cssNodes)
{
    std::vector<ComponentUI> componentUIs;
    
    seachHtmlNode(htmlNode,,0);
}