#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::function<std::pair<std::vector<double>, std::vector<double>>(const std::string&)> parseFunction;

class MeshParser
{
public:
    static std::pair<std::vector<double>, std::vector<double>> parse(const std::string& fileName);

private:
    static std::pair<std::vector<double>, std::vector<double>> parseSTL(const std::string& fileName);
    static std::vector<std::string> getFormats();
    static std::unordered_map<std::string, parseFunction> functionsMap;
    static std::vector<std::string> formats;
};

#endif // MESHPARSER_H
