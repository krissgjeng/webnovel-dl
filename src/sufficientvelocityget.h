#ifndef SUFFICIENTVELOCITY_H
#define SUFFICIENTVELOCITY_H
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include "novelget.h"

class SufficientVelocityGet : public NovelGet
{
public:
    SufficientVelocityGet(std::string url);
    virtual std::string getCh(int ch);
    virtual int GetChCount();
    virtual std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);
    virtual void getAllCb(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=1000);
    virtual void getChaptersCb(int from, int to, std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=1000);
    //virtual std::string GetAuthor();
    //virtual std::string GetTitle();
    
protected:
    virtual std::string getAuthorSrc(const std::string& source);
    virtual std::string getTitleSrc(const std::string& source);
    virtual std::string getChUrl(int ch, std::string url);
    //std::string url;
    //std::string author;
    //std::string title;
    
private:
    std::pair<int,std::string> pagehtml;
    std::pair<int, std::vector<std::string>> chapterBuffer;
    int chcount=-1;
    bool newUrlGet=false;
};
#endif // SUFFICIENTVELOCITY_H