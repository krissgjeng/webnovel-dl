#include "zipwrapper.h"
#include <iostream>
#include <string>
#ifdef __linux__
#include <dirent.h>
#else
#include "../windows/dirent.h"
#endif
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <filesystem>

#include <zip.h>

ZipWrapper::ZipWrapper()
{
}

static bool is_dir(const std::string &dir)
{
    struct stat st;
    ::stat(dir.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

static void walk_directory(const std::string &startdir, const std::string &inputdir, zip_t *zipper)
{
    DIR *dp = ::opendir(inputdir.c_str());
    if (dp == nullptr)
    {
        throw std::runtime_error("Failed to open input directory: " + std::string(::strerror(errno)));
    }

    struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name != std::string(".") && dirp->d_name != std::string(".."))
        {
            std::string fullname = inputdir + "/" + dirp->d_name;
            if (is_dir(fullname))
            {
                if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0)
                {
                    throw std::runtime_error("Failed to add directory to zip: " + std::string(zip_strerror(zipper)));
                }
                walk_directory(startdir, fullname, zipper);
            }
            else
            {
                zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
                if (source == nullptr)
                {
                    throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                }
                if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
                {
                    zip_source_free(source);
                    throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                }
            }
        }
    }
    ::closedir(dp);
}

void ZipWrapper::zip_directory(const std::string &inputdir, const std::string &output_filename)
{
    int errorp;
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }

    try
    {
        walk_directory(inputdir, inputdir, zipper);
    }
    catch (...)
    {
        zip_close(zipper);
        throw;
    }

    zip_close(zipper);
}
void ZipWrapper::zip_file(const std::string &inputfile, const std::string &output_filename)
{
    int errorp;
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }
    zip_source_t *source = zip_source_file(zipper, inputfile.c_str(), 0, 0);
    if (source == nullptr)
    {
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    auto filename = std::filesystem::path(inputfile).stem().string()+std::filesystem::path(inputfile).extension().string();
    if (zip_file_add(zipper, filename.c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
    {
        zip_source_free(source);
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    zip_close(zipper);
}
void ZipWrapper::zip_append_file(const std::string &inputfile, const std::string &output_filename)
{
    int errorp;
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }
    zip_source_t *source = zip_source_file(zipper, inputfile.c_str(), 0, 0);
    if (source == nullptr)
    {
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    auto filename = std::filesystem::path(inputfile).stem().string()+std::filesystem::path(inputfile).extension().string();
    if (zip_file_add(zipper, filename.c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
    {
        zip_source_free(source);
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    zip_close(zipper);
}
void ZipWrapper::zip_append_file_store(const std::string &inputfile, const std::string &internalfilepath, const std::string &output_filename)
{
    int errorp;
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }
    zip_source_t *source = zip_source_file(zipper, inputfile.c_str(), 0, 0);
    if (source == nullptr)
    {
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    if (zip_file_add(zipper, internalfilepath.c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
    {
        zip_source_free(source);
        throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
    }
    zip_set_file_compression(zipper, 0/*index*/, 0/*lvl*/, ZIP_CM_STORE/*type*/); //do not use compression on index 0
    zip_close(zipper);
}