#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <map>
#include "Color.h"

std::map<std::string, std::array<float, 4>> colors = {
    {"red", {1.0f, 0.0f, 0.0f, 1.0f}},
    {"green", {0.0f, 1.0f, 0.0f, 1.0f}},
    {"blue", {0.0f, 0.0f, 1.0f, 1.0f}},
    {"yellow", {1.0f, 1.0f, 0.0f, 1.0f}},
    {"cyan", {0.0f, 1.0f, 1.0f, 1.0f}},
    {"magenta", {1.0f, 0.0f, 1.0f, 1.0f}},
    {"white", {1.0f, 1.0f, 1.0f, 1.0f}},
    {"black", {0.0f, 0.0f, 0.0f, 1.0f}},
    {"orange", {1.0f, 0.5f, 0.0f, 1.0f}},
    {"purple", {0.5f, 0.0f, 0.5f, 1.0f}}};

int hexCharToInt(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('a' <= c && c <= 'f')
        return 10 + (c - 'a');
    if ('A' <= c && c <= 'F')
        return 10 + (c - 'A');
    return 0;
}

// Convert hex string #fff or #ffffff to RGBA array
std::array<float, 4> hexToRGBA(const std::string &hex)
{
    std::string h = hex;
    if (h[0] == '#')
        h = h.substr(1);

    std::array<int, 3> rgb;

    if (h.length() == 3)
    {
        // #RGB -> #RRGGBB
        for (int i = 0; i < 3; i++)
        {
            int val = hexCharToInt(h[i]);
            rgb[i] = val * 16 + val;
        }
    }
    else if (h.length() == 6)
    {
        for (int i = 0; i < 3; i++)
        {
            int hi = hexCharToInt(h[i * 2]);
            int lo = hexCharToInt(h[i * 2 + 1]);
            rgb[i] = hi * 16 + lo;
        }
    }
    else
    {
        rgb = {0, 0, 0};
    }

    return {rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f, 1.0f};
}

std::array<float,4> Color(const std::string &hex)
{
    if(hex[0] == '#'){
        return hexToRGBA(hex);
    }
    return colors[hex];
}