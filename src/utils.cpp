#include <yaml-cpp/yaml.h>
#include <iostream>

namespace Utils
{
    YAML::Node readConfig(const std::string &filePath)
    {
        try
        {
            YAML::Node config = YAML::LoadFile(filePath);
            return config;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error reading config file " << e.what() << "\n";
            return YAML::Node();
        }
    }
}
