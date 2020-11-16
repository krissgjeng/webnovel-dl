#include "fanfictionget.h"
#include <vector>
#define _GLIBCXX_HAS_GTHREADS
#include <chrono>
#include <thread>
#include "external/curlwrapper.h"
#include "stringhelper.h"
#include "console.h"

FanFictionGet::FanFictionGet(std::string url)
{
    //url should be https://www.fanfiction.net/s/12388283
    this->url = url;
    getChUrl(1); //inits stuff
}

std::string FanFictionGet::getAuthorSrc(const std::string& source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "By:", "</a>");
    int fi = authsrc.find("'>");
    authsrc = authsrc.substr(fi);
    return authsrc.substr(2);
}
std::string FanFictionGet::getTitleSrc(const std::string& source)
{
    using std::string;
    StringHelper str;
    string titlesrc = str.Get(source, "<title>", "</title>");
    titlesrc= str.Get(titlesrc, ">"," | FanFiction");
    str.replace(titlesrc," Chapter 1","");
    return titlesrc.substr(1);
}

std::string FanFictionGet::getChUrl(int ch)
{
    using std::vector, std::string;
    if (chapters.size() < 1)
    {
        CurlWrapper curl;
        StringHelper str;

        string source = curl.getSource(url);
        author = getAuthorSrc(source);
        title = getTitleSrc(source);
        string chaps = str.Get(source, "<select", "</select>",false,true);
        int chcount =str.CountStr(chaps, "<option");
        for (int i = 1; i <= chcount; i++)
        {
            chapters.push_back(url+"/"+std::to_string(i));
        }
    }
    if(ch-1<=chapters.size())
        return chapters[ch-1];
    else 
        return "";
}
std::string FanFictionGet::getCh(int ch)
{
    using std::vector, std::string;
    CurlWrapper curl;
    StringHelper str;
    string churl = getChUrl(ch);
    if(churl!="")
    {
            auto chsrc = curl.getSource(churl);
            //return chsrc = curl.getSource(chapters[ch-1]);
            return str.Get(chsrc, "<div role", "</div>")+"</div></div>";
    }
    else 
        return "";
}
int FanFictionGet::GetChCount()
{
    getChUrl(1);
    return chapters.size();
}
std::vector<std::string> FanFictionGet::getAllCh(std::function<void(int,std::string)> callback, int delayinms)
{
    using std::vector, std::string;

    vector<string> allchapters;
    string ch;
    int chi=1;
    while ( ( ch=getCh(chi) ) != "")
    {
        allchapters.push_back(ch);
        callback(chi,ch);
        Console::WriteLine(chi);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayinms));
        chi++;
    }

    return allchapters;
}
void FanFictionGet::getAllCb(std::function<void(int, std::string)> callback, int delayinms)
{
    int to = GetChCount();
    getChaptersCb(1,to,callback,delayinms);
}
void FanFictionGet::getChaptersCb(int from, int to, std::function<void(int, std::string)> callback, int delayinms)
{
    using std::string;

    string ch;
    for (size_t i = from; i <= to; i++)
    {
        ch = getCh(i);
        callback(i, ch);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayinms));
    }
}