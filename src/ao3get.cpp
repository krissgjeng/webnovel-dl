#include "ao3get.h"
#include <vector>
#define _GLIBCXX_HAS_GTHREADS
#include <chrono>
#include <thread>
#include "external/curlwrapper.h"
#include "stringhelper.h"
#include "console.h"
#include <iostream>
#include <fstream>

//todo add parseurl to get correct url format if it differs from expected
AO3Get::AO3Get(std::string url)
{
    //url should be https://archiveofourown.org/works/11478249/chapters/25740126
    this->url = url+"?view_adult=true";
    getChUrl(1); //inits stuff
    Console::WriteLine("ao3init done");
}
std::string AO3Get::convertUrl(std::string url)
{
    //from: https://archiveofourown.org/works/11478249/chapters/25740126
    //to https://archiveofourown.org/works/11478249/chapters/
    std::string newurl = url.substr(0,url.rfind("/"));
    //Console::WriteLine(newurl);
    return newurl;
}

std::string AO3Get::getAuthorSrc(const std::string& source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "<a rel=\"author", "</a>");
    //Console::WriteLine(authsrc);
    int fi = authsrc.find(">");
    authsrc = authsrc.substr(fi);
    return authsrc.substr(1);
}
std::string AO3Get::getTitleSrc(const std::string& source)
{
    using std::string;
    StringHelper str;
    Console::WriteLine("start title");
    //Console::WriteLine(source);
    string titlesrc = str.Get(source, "<h2 class=\"title", "</h2>");
    int fi = titlesrc.find(">");
    titlesrc = titlesrc.substr(fi);
    return titlesrc.substr(1);
}

std::string AO3Get::getChUrl(int ch)
{
    using std::vector, std::string;
    if (chapters.size() < 1)
    {
        CurlWrapper curl;
        StringHelper str;

        string source = curl.getSource(url);
        author = getAuthorSrc(source);
        title = getTitleSrc(source);
        string chaps = str.Get(source, "<ul id=\"chapter_index", "</ul>");
        auto chaplist = str.GetAll(chaps,"<option ",">",true);
        for (size_t i = 0; i < chaplist.size(); i++)
        {
            string chid=str.Get(chaplist[i],"value=\"","\">").substr(7);
            str.replace(chid,"\"","");
            chapters.push_back(convertUrl(this->url)+"/"+  chid+"?view_adult=true");
            //Console::WriteLine(chapters[i]);
        }
    }
    if(ch-1<=chapters.size())
        return chapters[ch-1];
    else 
        return "";
}
std::string AO3Get::getCh(int ch)
{
    using std::vector, std::string;
    CurlWrapper curl;
    StringHelper str;
    string churl = getChUrl(ch);
    if(churl!="")
    {
            auto chsrc = curl.getSource(churl);
            //return chsrc = curl.getSource(chapters[ch-1]);
            string start="<!--main content-->";
            string content = str.Get(chsrc, start, "<!--/main-->").substr(start.length());
            return content;
    }
    else 
        return "";
}
int AO3Get::GetChCount()
{
    getChUrl(1);
    return chapters.size();
}
std::vector<std::string> AO3Get::getAllCh(std::function<void(int,std::string)> callback, int delayinms)
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
void AO3Get::getAllCb(std::function<void(int, std::string)> callback, int delayinms)
{
    int to = GetChCount();
    getChaptersCb(1,to,callback,delayinms);
}
void AO3Get::getChaptersCb(int from, int to, std::function<void(int, std::string)> callback, int delayinms)
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