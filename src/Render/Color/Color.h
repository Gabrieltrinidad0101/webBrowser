#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <string>
#include <map>


using RGBA = std::array<float,4>;

extern std::map<std::string, RGBA> colors;

// Function that returns the RGBA array for a given hex string
RGBA Color(const std::string &hex);

#endif // COLOR_H
