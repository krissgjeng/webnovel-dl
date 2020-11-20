#ifndef AO3GET_H
#define AO3GET_H
#include <string>
#include <vector>
#include <functional>
#include "novelget.h"

class AO3Get : public NovelGet
{
public:
    AO3Get(std::string url);
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
std::string convertUrl(std::string url);
    std::vector<std::string> chapters;
};
#endif // AO3GET_H