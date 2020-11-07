#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#include <vector>

class StringHelper
{
public:
    StringHelper();
    static std::string Get(const std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive=false);
    static std::vector<std::string> GetAll(std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive=false);
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);
    static void replace(std::string& str, const std::string& from, const std::string& to);
    static int GetNthIndex(const std::string& source, const std::string &substring,int nth);
    static void RemoveIllegalCharsInFsName(std::string& source);
    static std::string UnescapeHtmlEntities(const std::string& source);
private:

};
#endif // STRINGHELPER_H