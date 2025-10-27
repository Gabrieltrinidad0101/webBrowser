#include "ApplyCssToHtml.h"
#include "ComponentUI/ComponentUI.h"
#include "../Utils/print.h"
#include <iostream>
#include <string>
#include <set>
#include <map>

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
        while (i < str.size() && str[i] != ' ')
        {
            class_ += str[i];
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

static const std::map<std::string, DISPLAY> displayMap{
    {"flex", DISPLAY::FLEX},
    {"grid", DISPLAY::GRID},
    {"inline", DISPLAY::INLINE},
    {"block", DISPLAY::BLOCK}};

void ApplyCssToHtml::applyCssToTag(HtmlNode *htmlNode)
{
}

ComponentUI *ApplyCssToHtml::htmlNodeToComponentUI(HtmlNode *htmlNode, std::vector<CssNode> &cssesNode,std::map<int,int> cssIndexQueryIndex)
{
    ComponentUI componentUI;
    componentUI.tag = htmlNode->tag;
    for(size_t i = 0; i < cssesNode.size(); i++){
        Query query = cssesNode[i].queries[cssIndexQueryIndex[i]];
        if ((query.queryType == QUERY_TYPE::TAG && htmlNode->tag == query.toSearch) ||
            (query.queryType == QUERY_TYPE::ID &&
             htmlNode->attributes.find("id") != htmlNode->attributes.end() &&
             htmlNode->attributes.at("id") == query.toSearch))
        {
            cssIndexQueryIndex[i] += 1;
        }
    
        else if (query.queryType == QUERY_TYPE::CLASS)
        {
            std::set<std::string> classes = getClasses(htmlNode->attributes["class"]);
            if (classes.count(query.toSearch) == 1)
            {
                cssIndexQueryIndex[i] += 1;
            }
        }
        if (cssIndexQueryIndex[i] == cssesNode[i].queries.size())
        {
            applyAndAnalyze(&componentUI, cssesNode[i].attributes);
            cssIndexQueryIndex[i] -= 1;
        }
    }


    for (auto &child : htmlNode->children)
    {
        ComponentUI *componentUIChild = htmlNodeToComponentUI(child, cssesNode, cssIndexQueryIndex);
        componentUI.children.push_back(componentUIChild);
    }
    componentUI.build();
    return &componentUI;
}

void ApplyCssToHtml::applyAndAnalyze(ComponentUI *componentUI, std::map<std::string, std::string> attributes)
{
    if(componentUI->tag == "div"){
        componentUI->cssStyle.display = DISPLAY::BLOCK;
    }
    for (const auto &attribute : attributes)
    {
        if (attribute.first == "width")
        {
            componentUI->cssStyle.width = removeEnd(attribute.second, "px");
        }

        if (attribute.first == "height")
        {
            componentUI->cssStyle.height = removeEnd(attribute.second, "px");
        }

        if (attribute.first == "margin-left")
        {
            componentUI->cssStyle.x = removeEnd(attribute.second, "px");
        }

        if (attribute.first == "margin-top")
        {
            componentUI->cssStyle.y = removeEnd(attribute.second, "px");
        }

        if(attribute.first == "display"){
            if(attribute.second == "flex"){
                componentUI->cssStyle.display = DISPLAY::FLEX;
            }
            if(attribute.second == "grid"){
                componentUI->cssStyle.display = DISPLAY::GRID;
            }
            if(attribute.second == "inline"){
                componentUI->cssStyle.display = DISPLAY::INLINE;
            }
            if(attribute.second == "block"){
                componentUI->cssStyle.display = DISPLAY::BLOCK;
            }
        }

        if(attribute.first == "align-item"){
            if(attribute.second == "center"){
                componentUI->cssStyle.alignItems = ALIGN_ITEMS::CENTER;
            }
            if(attribute.second == "flex-start"){
                componentUI->cssStyle.alignItems = ALIGN_ITEMS::FLEX_START;
            }
            if(attribute.second == "flex-end"){
                componentUI->cssStyle.alignItems = ALIGN_ITEMS::FLEX_END;
            }
        }

        if (attribute.first == "justify-content")
        {
            if(attribute.second == "space-between"){
                componentUI->cssStyle.justifyContent = JUSTIFY_CONTENT::SPACE_BETWEEN;
            }
            if(attribute.second == "center"){
                componentUI->cssStyle.justifyContent = JUSTIFY_CONTENT::CENTER;
            }
        }

        if (attribute.first == "background-color")
        {
            componentUI->cssStyle.bgColor = Color(attribute.second);
        }
    }
}

void ApplyCssToHtml::convertTreeToArray(ComponentUI *componentUI, std::vector<ComponentUI *> &componentUIs)
{
    for(ComponentUI* child : componentUI->children){
        componentUIs.push_back(child);
        convertTreeToArray(child, componentUIs);
    }
}


std::vector<ComponentUI *> ApplyCssToHtml::applyCss(HtmlNode* htmlNode, std::vector<CssNode> cssNodes)
{
    std::map<int,int> cssIndexQueryIndex;
    for(size_t i = 0; i < cssNodes.size(); i++){
        cssIndexQueryIndex[i] = 0;
    }
    std::vector<ComponentUI*> componentUIs;
    ComponentUI* componentUI = htmlNodeToComponentUI(htmlNode, cssNodes, cssIndexQueryIndex);
    printComponentUI(componentUI);
    return componentUIs;
}