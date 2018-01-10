#include "meshparser.h"

#include "exceptions.h"

std::unordered_map<std::string, parseFunction> MeshParser::functionsMap = {std::make_pair(".stl", &MeshParser::parseSTL)};
std::vector<std::string> MeshParser::formats = MeshParser::getFormats();

std::pair<std::vector<double>, std::vector<double>> MeshParser::parse(const std::string &fileName)
{
    if (fileName.size() > 4)
    {
        std::string format = fileName.substr(fileName.size()-4, 4);
        for (auto p: functionsMap)
        {
            if (format == p.first)
            {
                return p.second(fileName);
            }
        }
    }
    throw FileFormatError(fileName, formats);
}

std::pair<std::vector<double>, std::vector<double>> MeshParser::parseSTL(const std::string &fileName)
{
    std::ifstream in(fileName);
    if (in.is_open())
    {
        std::string str;
        std::vector<double> coord, norm;
        in >> str;
        if (str == "solid")
        {
            while (str != "facet") in >> str;
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            while (str != "endsolid")
            {
                if (str == "facet")
                {
                    try
                    {
                        in >> str >> x1 >> y1 >> z1;
                        norm.push_back(x1);
                        norm.push_back(y1);
                        norm.push_back(z1);
                        norm.push_back(x1);
                        norm.push_back(y1);
                        norm.push_back(z1);
                        norm.push_back(x1);
                        norm.push_back(y1);
                        norm.push_back(z1);
                    }
                    catch (std::exception&)
                    {
                        throw FileParsingError(fileName, "ASCII STL formal invalid");
                    }
                }
                else if (str == "outer")
                {
                    try
                    {
                        in >> str >> str >> x1 >> y1 >> z1 >> str >> x2 >> y2 >> z2 >> str >> x3 >> y3 >> z3;
                        coord.push_back(x1);
                        coord.push_back(y1);
                        coord.push_back(z1);
                        coord.push_back(x2);
                        coord.push_back(y2);
                        coord.push_back(z2);
                        coord.push_back(x3);
                        coord.push_back(y3);
                        coord.push_back(z3);
                    }
                    catch (std::exception&)
                    {
                        throw FileParsingError(fileName, "ASCII STL formal invalid");
                    }
                }
                else if (str != "endloop" && str != "endfacet" && !str.empty())
                {
                    throw FileParsingError(fileName, "ASCII STL formal invalid");
                }
                in >> str;
            }
            return std::make_pair(coord, norm);
        } // TODO: binary STL
        else throw FileParsingError(fileName, "Only ASCII STL files supported");
    }
    else throw FileOpeningFailure(fileName);

}

std::vector<std::string> MeshParser::getFormats()
{
    std::vector<std::string> res;

    std::transform(functionsMap.begin(), functionsMap.end(),
                   std::back_inserter(res), [](std::pair<std::string, parseFunction> p)->std::string
                                                        {
                                                            return p.first;
                                                        });
    return res;
}
