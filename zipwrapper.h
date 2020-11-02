#ifndef ZIPWRAPPER_H
#define ZIPWRAPPER_H
#include <string>

class ZipWrapper
{
public:
    ZipWrapper();
    static void zip_directory(const std::string& inputdir, const std::string& output_filename);
    static void zip_file(const std::string& inputfile, const std::string& output_filename);
    static void zip_append_file(const std::string& inputfile, const std::string& output_filename);
    static void zip_append_file_store(const std::string& inputfile, const std::string& internalfilepath, const std::string& output_filename);
private:

};
#endif // ZIPWRAPPER_H