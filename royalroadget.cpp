#include "royalroadget.h"
#include <vector>
#define _GLIBCXX_HAS_GTHREADS
#include <chrono>
#include <thread>
#include "curlwrapper.h"
#include "stringhelper.h"
#include "console.h"

RoyalRoadGet::RoyalRoadGet(std::string url)
{
    this->url = url;
}

std::string RoyalRoadGet::getChUrl(int ch)
{
    using std::vector, std::string;
    if (chapters.size() < 1)
    {
        CurlWrapper curl;
        StringHelper str;

        string source = curl.getSource(url);
        string chaps = str.Get(source, "<tbody>", "</tbody>");

        chapters = str.GetAll(chaps, "href", "\">");
        for (int i = 0; i < chapters.size(); i++)
        {
            str.replaceAll(chapters[i], "href=\"", "https://www.royalroad.com");
            //Console::WriteLine(a[i]);
        }
    }
    if(ch-1<=chapters.size())
        return chapters[ch-1];
    else 
        return "";
}
std::string RoyalRoadGet::getCh(int ch)
{
    using std::vector, std::string;
    CurlWrapper curl;
    StringHelper str;
    string churl = getChUrl(ch);
    if(churl!="")
    {
            auto chsrc = curl.getSource(churl);
            //return chsrc = curl.getSource(chapters[ch-1]);
            return str.Get(chsrc, "<div class=\"chapter-inner", "<h6 class");
    }
    else 
        return "";
}
int RoyalRoadGet::GetChCount()
{
    getChUrl(1);
    return chapters.size();
}
std::vector<std::string> RoyalRoadGet::getAllCh(std::function<void(int,std::string)> callback, int delayinms)
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