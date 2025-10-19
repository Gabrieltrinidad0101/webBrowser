#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <type_traits>
#include <map>
#include "ComponentUI/ComponentUI.h"
#include "ParserHtml/ParserHtml.h"

// Helper to detect getProperties
template <typename T, typename = void>
struct has_getProperties : std::false_type
{
};

template <typename T>
struct has_getProperties<T, std::void_t<decltype(std::declval<T>().getProperties())>> : std::true_type
{
};

template <typename T = int>
std::string valueToString(std::any value)
{
    if (value.type() == typeid(int))
        return std::to_string(std::any_cast<int>(value));
    if (value.type() == typeid(size_t))
        return std::to_string(std::any_cast<size_t>(value));
    if (value.type() == typeid(float))
        return std::to_string(std::any_cast<float>(value));

    if (value.type() == typeid(bool))
        return std::to_string(std::any_cast<bool>(value));

    if (value.type() == typeid(std::string))
        return std::any_cast<std::string>(value);

    if (value.type() == typeid(const char *))
        return std::string(std::any_cast<const char *>(value));

    if (value.type() == typeid(char))
        return std::string(1, std::any_cast<char>(value));
    if (value.type() == typeid(std::vector<std::any>))
    {
        const auto &vec = std::any_cast<const std::vector<std::any> &>(value);
        std::string res = "[";
        bool first = true;
        for (const auto &v : vec)
        {
            if (!first)
                res += ", ";
            first = false;
            res += valueToString<int>(v);
        }
        res += "]";
        return res;
    }

    if (value.type() == typeid(std::vector<std::string>))
    {
        auto vec = std::any_cast<std::vector<std::string>>(value);
        std::string res = "[";
        bool first = true;
        for (auto v : vec)
        {
            if (!first)
                res += ", ";
            first = false;
            res += valueToString<int>(v);
        }
        res += "]";
        return res;
    }

    // Only try casting to T if T has getProperties()
    if constexpr (has_getProperties<T>::value)
    {
        try
        {
            const T &obj = std::any_cast<const T &>(value);
            std::string res = typeid(T).name();
            res += " {";
            bool first = true;
            for (auto &[key, val] : obj.getProperties())
            {
                if (!first)
                    res += ", ";
                first = false;
                res += key + ": " + valueToString<int>(val);
            }
            res += "}";
            return res;
        }
        catch (const std::bad_any_cast &)
        {
            return "[unknown class]";
        }
    }

    return "[unknown type]";
}

inline void printVectorString(std::vector<std::string> value)
{
    std::string res = "[";
    bool first = true;
    for (const auto &v : value)
    {
        if (!first)
            res += ", ";
        first = false;
        res += v;
    }
    res += "]";
    std::cout << res << std::endl;
}

inline void printComponentUI(std::vector<ComponentUI *> value)
{
    std::string res = "[";
    bool first = true;
    for (const auto &v : value)
    {
        if (!first)
            res += ", ";
        first = false;
        res += valueToString(v);
    }
    res += "]";
    std::cout << res << std::endl;
}

template <typename T = int>
void print(const std::any &value)
{
    std::cout << valueToString<T>(value) << std::endl;
}


inline void printTree(HtmlNode htmlNode,std::string str1 = ""){
    std::cout << str1 << htmlNode.tag << std::endl;
    for(HtmlNode child : htmlNode.children){
        str1 += "  ";
        printTree(child,str1);
    }
}