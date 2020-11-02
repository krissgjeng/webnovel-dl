#ifndef MAKEEPUB_H
#define MAKEEPUB_H
#include <string>
#include <vector>

class MakeEpub
{
public:
    MakeEpub(std::string outputdir="/pub");
    void BuildEpub(std::string url);
private:
    std::string outDir;
    std::string workDir;
    std::string tmpDir;
    std::vector<std::string> htmlfiles;
    int chfrom;
    int chto;
    int chapters;
    void prepareStructure();
    void downloadChapters(std::string url, int from=1, int to=-1);
    void convertToXhtml();
    void createToc();
    void compressContent();
    

};
#endif // MAKEEPUB_H