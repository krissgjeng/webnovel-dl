#ifndef ROYALROADGET_H
#define ROYALROADGET_H
#include <string>
#include <vector>
#include <functional>

class RoyalRoadGet
{
public:
    RoyalRoadGet(std::string url);
    std::string getCh(int ch);
    std::vector<std::string> getAllCh(std::function<void(int,std::string)> callback=[](int,std::string){},int delayinms=2000);
private:
    std::string getChUrl(int ch);
    std::string url;
    std::vector<std::string> chapters;
};
#endif // ROYALROADGET_H