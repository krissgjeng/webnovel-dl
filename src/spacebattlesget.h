#ifndef SPACEBATTLESGET_H
#define SPACEBATTLESGET_H
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include "novelget.h"

class SpaceBattlesGet : public NovelGet
{
public:
    SpaceBattlesGet(std::string url);
    virtual std::string getCh(int ch);
    virtual int GetChCount();
    virtual std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);
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
};
#endif // SPACEBATTLESGET_H