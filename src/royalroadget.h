#ifndef ROYALROADGET_H
#define ROYALROADGET_H
#include <string>
#include <vector>
#include <functional>
#include "novelget.h"

class RoyalRoadGet : public NovelGet
{
public:
    RoyalRoadGet(std::string url);
    virtual std::string getCh(int ch);
    virtual int GetChCount();
    virtual std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);
    //virtual std::string GetAuthor();
    //virtual std::string GetTitle();
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
#endif // ROYALROADGET_H