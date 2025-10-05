#ifndef PARSER_CSS_H
#define PARSER_CSS_H

#include <string>
#include <vector>
#include <map>
#include "Color/Color.h"
#include <any>

struct CssLexer
{
    std::string query;
    std::map<std::string, std::string> attributes;
};

enum QUERY_TYPE
{
    ID,
    CLASS,
    TAG,
};

enum QUERY_OPERATION
{
    ONLY_CHILDREN,
    ALL_CHILDREN
};

enum DISPLAY
{
    FLEX,
    GRID,
    INLINE,
    BLOCK
};


enum class JUSTIFY_CONTENT
{
    CENTER,
    SPACE_BETWEEN,
    SPACE_EVENLY,
    SPACE_AROUND
};

enum class ALIGN_ITEMS
{
    STRETCH,
    CENTER,
    FLEX_START,
    FLEX_END
};

struct Query
{
    QUERY_TYPE queryType;
    std::string toSearch;
    QUERY_OPERATION queryOperation;
};

struct CssStyle {
    size_t width;
    size_t height;

    // position
    DISPLAY display;
    JUSTIFY_CONTENT justifyContent;
    ALIGN_ITEMS alignItems;

    std::array<float, 4> color;
    RGBA bgColor;

    // border color
    RGBA borderColor;
    RGBA borderLeftColor;
    RGBA borderRightColor;
    RGBA borderTopColor;
    RGBA borderBottomColor;

    // border width
    size_t borderLeftWidth = 0;
    size_t borderRightWidth = 0;
    size_t borderTopWidth = 0;
    size_t borderBottomWidth = 0;
};

struct CssNode
{
    std::vector<Query> queries;
    CssStyle cssStyle;

    std::map<std::string, std::any> getProperties() const {
        return {
            {"width", cssStyle.width},
            {"height", cssStyle.height},
            {"display", cssStyle.display},
            {"justifyContent", cssStyle.justifyContent},
            {"alignItems", cssStyle.alignItems},
            {"color", cssStyle.color},
            {"bgColor", cssStyle.bgColor},
            {"borderColor", cssStyle.borderColor},
            {"borderLeftColor", cssStyle.borderLeftColor},
            {"borderRightColor", cssStyle.borderRightColor},
            {"borderTopColor", cssStyle.borderTopColor},
            {"borderBottomColor", cssStyle.borderBottomColor},
            {"borderLeftWidth", cssStyle.borderLeftWidth},
            {"borderRightWidth", cssStyle.borderRightWidth},
            {"borderTopWidth", cssStyle.borderTopWidth},
            {"borderBottomWidth", cssStyle.borderBottomWidth},
        };
    }
};

class ParserCss
{
public:
    std::vector<CssNode> parser(std::vector<std::string> csses);
    std::map<std::string, std::any> getProperties() const;

private:
    std::vector<CssLexer> lexer(const std::string &css);
    std::vector<Query> queryParser(const std::string &css);
    void jumpSpace(const std::string &css, size_t &i);
};

#endif // PARSER_CSS_H