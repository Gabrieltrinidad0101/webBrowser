#include "ParserHtml.h"
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>

void ParserHtml::jumpSpace(const std::string &html, size_t &i) {
    while (i < html.size() && (html[i] == ' ' || html[i] == '\n' || html[i] == '\t' || html[i] == '\r'))
        i++;
}

std::vector<HtmlNode> ParserHtml::lexer(const std::string &html) {
    std::vector<HtmlNode> tokens;
    std::set<std::string> voidTags = {"area","base","br","col","embed","hr","img","input","link","meta","param","source","track","wbr"};

    size_t i = 0;
    while (i < html.size()) {
        jumpSpace(html, i);

        if (i >= html.size()) break;

        if (html[i] == '<') {
            i++;
            bool isOpen = html[i] != '/';
            if (!isOpen) i++;

            std::string tag;
            while (i < html.size() && html[i] != ' ' && html[i] != '>' && html[i] != '/') {
                tag += html[i];
                i++;
            }

            jumpSpace(html, i);

            std::map<std::string, std::string> attributes;
            while (i < html.size() && html[i] != '>' && html[i] != '/') {
                jumpSpace(html, i);
                if (html[i] == '>' || html[i] == '/') break;

                std::string key, value;
                while (i < html.size() && html[i] != ' ' && html[i] != '=') {
                    key += html[i];
                    i++;
                }

                jumpSpace(html, i);
                if (i < html.size() && html[i] == '=') i++; // skip '='
                jumpSpace(html, i);

                char quote = '\0';
                if (i < html.size() && (html[i] == '"' || html[i] == '\'')) {
                    quote = html[i];
                    i++;
                }

                while (i < html.size() && html[i] != ' ' && html[i] != '>' && (quote == '\0' || html[i] != quote)) {
                    value += html[i];
                    i++;
                }

                if (quote != '\0' && i < html.size() && html[i] == quote) i++; // skip closing quote

                if (!key.empty()) attributes[key] = value;
            }

            bool selfClosing = false;
            if (i < html.size() && html[i] == '/') {
                selfClosing = true;
                i++;
            }
            if (i < html.size() && html[i] == '>') i++;

            std::string innerText;
            if (isOpen && !selfClosing && voidTags.find(tag) == voidTags.end()) {
                size_t textStart = i;
                while (i < html.size() && html[i] != '<') i++;
                innerText = html.substr(textStart, i - textStart);
            }

            HtmlNode node{tag, innerText, attributes, isOpen: true};
            tokens.push_back(node);
            if (!isOpen || selfClosing || voidTags.find(tag) != voidTags.end()) {
                HtmlNode closeNode{tag, "", {}, isOpen: false};
                tokens.push_back(closeNode);
            }
        } else {
            i++;
        }
    }

    return tokens;
}

void ParserHtml::parser(const std::string &html) {
    HtmlNode root{parent: nullptr, isOpen: true};
    HtmlNode* current = &root;
    std::vector<HtmlNode> tokens = lexer(html);

    for (HtmlNode& token : tokens) {
        token.parent = current;

        if (!token.isOpen) {
            if (current->parent) current = current->parent;
        } else {
            current->children.push_back(token);
            current = &current->children.back();
        }
    }
}