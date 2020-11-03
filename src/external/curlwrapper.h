#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H
#include <string>

class CurlWrapper
{
public:
    CurlWrapper();
    std::string getSource(std::string url);
private:

};
#endif // CURLWRAPPER_H