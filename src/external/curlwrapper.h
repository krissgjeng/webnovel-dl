#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H
#include <string>

class CurlWrapper
{
public:
    CurlWrapper();
    std::string getSource(std::string url);
    std::string urlDecode(const std::string& string);
    std::string urlEncode(const std::string& string);
private:

};
#endif // CURLWRAPPER_H