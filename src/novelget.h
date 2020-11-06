#ifndef NOVELGET_H
#define NOVELGET_H
#include <string>
#include <vector>
#include <functional>

class NovelGet
{
public:
    //NovelGet(std::string url);
    virtual ~NovelGet(){};
    //virtual void init(std::string url) =0;
    virtual std::string getCh(int ch)=0;
    virtual int GetChCount()=0;
    virtual std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000)=0;

    virtual std::string GetAuthor();
    virtual std::string GetTitle();
protected:
    virtual std::string getAuthorSrc(const std::string& source)=0;
    virtual std::string getTitleSrc(const std::string& source)=0;
    //virtual std::string getChUrl(int ch)=0;
    //std::vector<std::string> chapters;
    std::string url="";
    std::string title="";
    std::string author="";
};
#endif // NOVELGET_H