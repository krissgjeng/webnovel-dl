#ifndef MAKEEPUB_H
#define MAKEEPUB_H
#include <string>
#include <vector>

class NovelGet;

class MakeEpub
{
public:
    MakeEpub(NovelGet* ng, std::string outputdir="/pub");
    void BuildEpub(std::string url);
    void BuildEpub(std::string url, int from, int to=-1);
private:
    std::string outDir;
    std::string workDir;
    std::string tmpDir;
    std::vector<std::string> htmlfiles;
    int chfrom;
    int chto;
    int chapters;
    NovelGet* ng=nullptr;
    void prepareStructure();
    void downloadChapters(std::string url, int from=1, int to=-1);
    void convertToXhtml();
    void createToc();
    void compressContent();
    void cleanup();
    
};
#endif // MAKEEPUB_H