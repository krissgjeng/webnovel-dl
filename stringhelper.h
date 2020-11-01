#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#include <vector>

class StringHelper
{
public:
    StringHelper();
    static std::string Get(std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive=false);
    static std::vector<std::string> GetAll(std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive=false);
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);
    static void replace(std::string& str, const std::string& from, const std::string& to);
private:

};
#endif // STRINGHELPER_H