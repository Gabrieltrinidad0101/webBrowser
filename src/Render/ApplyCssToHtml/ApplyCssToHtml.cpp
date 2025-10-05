#include "ApplyCssToHtml.h"
#include "ComponentUI/ComponentUI.h"
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

size_t removeEnd(std::string s, std::string toReplace)
{
    if (s.size() > toReplace.size() && s.substr(s.size() - toReplace.size()) == toReplace)
        s = s.substr(0, s.size() - toReplace.size());
    return std::stoul(s);
}

static const std::map<std::string, DISPLAY> displayMap {
    {"flex", DISPLAY::FLEX},
    {"grid", DISPLAY::GRID},
    {"inline", DISPLAY::INLINE},
    {"block", DISPLAY::BLOCK}};

void ApplyCssToHtml::applyCssToTag(HtmlNode *htmlNode)
{
}

ComponentUI& ApplyCssToHtml::htmlNodeToComponentUI(HtmlNode &htmlNode,CssNode &cssNode, size_t queryIndex)
{
    ComponentUI componentUI;
    Query query = cssNode.queries[queryIndex];
    if (
        (query.queryType == QUERY_TYPE::TAG && htmlNode.tag == query.toSearch) ||
        (query.queryType == QUERY_TYPE::ID && htmlNode.attributes["id"] == query.toSearch))
    {
        queryIndex++;
    }
    else if (query.queryType == QUERY_TYPE::CLASS)
    {
        std::set<std::string> classes = getClasses(htmlNode.attributes["class"]);
        classes.count(query.toSearch);
        queryIndex++;
    }

    if (queryIndex == cssNode.queries.size())
    {
        applyAndAnalyze(componentUI,cssNode.attributes);
        queryIndex--;
    }

    for (auto &child : htmlNode.children)
    {
        ComponentUI& componentUIChild = htmlNodeToComponentUI(child, cssNode, queryIndex);
        componentUI.children.push_back(componentUIChild);
    }
    return componentUI;
}

void ApplyCssToHtml::applyAndAnalyze(ComponentUI componentUI,std::map<std::string, std::string> attributes)
{
    for (const auto &attribute : attributes)
    {
        if (attribute.first == "width")
        {
            componentUI.cssStyle.width = removeEnd(attribute.second, "px");
        }

        if (attribute.first == "height")
        {
            componentUI.cssStyle.height = removeEnd(attribute.second, "px");
        }

        if (attribute.first == "background-color")
        {
            componentUI.cssStyle.bgColor = Color(attribute.second);
        }
        auto it = displayMap.find(attribute.first);
        if (it != displayMap.end())
            componentUI.cssStyle.display = it->second;
    }
}

void ApplyCssToHtml::applyCss(HtmlNode *htmlNode, std::vector<CssNode> cssNodes)
{

    std::vector<ComponentUI &> componentUIs;
    for (auto &cssNode : cssNodes)
    {
        componentUIs = htmlNodeToComponentUI(htmlNode, cssNode, 0);
    }
}