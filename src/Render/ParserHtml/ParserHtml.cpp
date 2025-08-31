#include "ParserHtml.h"
#include <vector>
#include <iostream>

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
            bool isOpen = html[i] != '/';
            if (!isOpen)
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
                i--;
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

std::ostream& operator<<(std::ostream& os, const HtmlNode& node) {
    os << "HtmlNode{tag:" << node.tag
       << ", isOpen:" << (node.isOpen ? "true" : "false");
    if (node.isOpen) {
        os << ", innerText:" << node.innerText;
        os << ", attributes:{";
        for (auto& kv : node.attributes)
            os << kv.first << ":" << kv.second << " ";
        os << "}";
    }
    os << "}";
    return os;
}

// Función genérica para imprimir cualquier elemento simple
template <typename T>
typename std::enable_if<!std::is_class<T>::value, void>::type
print(const T& value) {
    std::cout << value << " ";
}

// Función genérica para imprimir cualquier objeto con operator<<
template <typename T>
typename std::enable_if<std::is_class<T>::value, void>::type
print(const T& obj) {
    std::cout << obj << " ";
}

// Función genérica para imprimir contenedores
template <typename Container>
void printContainer(const Container& c) {
    for (const auto& elem : c) {
        print(elem);
    }
    std::cout << std::endl;
}

void ParserHtml::parser(const std::string &html)
{
    HtmlNode root{
        parent :nullptr, 
        isOpen: true
    };

    HtmlNode* current = &root;
    std::vector<HtmlNode> tokens = this->lexer(html);

    for (HtmlNode& token : tokens) {
        token.parent = current;

        if (!token.isOpen) {
            if (current->parent != nullptr) {
                current = current->parent;
            }
        } else {
            current->children.push_back(token);
            current = &current->children.back();
        }
    }
}