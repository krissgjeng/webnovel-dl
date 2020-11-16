#ifndef FANFICTIONGET_H
#define FANFICTIONGET_H
#include <string>
#include <vector>
#include <functional>
#include "novelget.h"

class FanFictionGet : public NovelGet
{
public:
    FanFictionGet(std::string url);
    virtual std::string getCh(int ch);
    virtual int GetChCount();
    virtual std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);
    virtual void getAllCb(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=1000);
    virtual void getChaptersCb(int from, int to, std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=1000);
protected:
    virtual std::string getAuthorSrc(const std::string& source);
    virtual std::string getTitleSrc(const std::string& source);
    virtual std::string getChUrl(int ch);
    //std::string url="";
    //std::string title="";
    //std::string author="";
    
private:
    std::vector<std::string> chapters;
};
#endif // FANFICTIONGET_H