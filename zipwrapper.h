#ifndef ZIPWRAPPER_H
#define ZIPWRAPPER_H
#include <string>

class ZipWrapper
{
public:
    ZipWrapper();
    static void zip_directory(const std::string& inputdir, const std::string& output_filename);
private:

};
#endif // ZIPWRAPPER_H