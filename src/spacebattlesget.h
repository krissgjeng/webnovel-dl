#ifndef SPACEBATTLESGET_H
#define SPACEBATTLESGET_H
#include <string>
#include <vector>
#include <functional>
#include <utility>

class SpaceBattlesGet
{
public:
    SpaceBattlesGet(std::string url);
    std::string getCh(int ch);
    int GetChCount();
    std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);

    std::string Author;
    std::string Title;
private:
    std::string getAuthor(const std::string& source);
    std::string getTitle(const std::string& source);
    std::string getChUrl(int ch, std::string url);
    std::string url;
    std::vector<std::string> chapters;
    std::pair<int,std::string> pagehtml;
};
#endif // SPACEBATTLESGET_H