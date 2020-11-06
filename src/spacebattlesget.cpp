#include "spacebattlesget.h"
#include "external/curlwrapper.h"
#include "stringhelper.h"
#include "console.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#define _GLIBCXX_HAS_GTHREADS
#include <thread>

//not done
SpaceBattlesGet::SpaceBattlesGet(std::string url)
{
    this->url = url;
    //getChUrl(1); //inits stuff

    //CurlWrapper curl;
    //std::string source = curl.getSource(url);
    //author = getAuthorSrc(source);
    //title = getTitleSrc(source);
    author="not impl";
    title="not impl";
}

std::string SpaceBattlesGet::getAuthorSrc(const std::string &source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "<h4", "</h4>");
    authsrc = str.Get(authsrc, "<a href=", "/a>");
    authsrc = str.Get(authsrc, ">", "<");
    return authsrc.substr(1);
}
std::string SpaceBattlesGet::getTitleSrc(const std::string &source)
{
    using std::string;
    StringHelper str;
    string titlesrc = str.Get(source, "<h1", "/h1>");
    return str.Get(titlesrc, ">", "<").substr(1);
}

std::string SpaceBattlesGet::getChUrl(int ch, std::string url)
{
    using std::string, std::pair;
    CurlWrapper curl;
    StringHelper str;

    int page = ((ch - 1) / 10);
    int pagepart = ((ch - 1) % 10);
    if (pagepart == 1)
    {
        if ((pagehtml == pair<int, string>() || page != pagehtml.first) && page <= 1)
            pagehtml = {page, curl.getSource(url)};
        return this->url;
    }
    if (pagehtml == pair<int, string>() || page != pagehtml.first)
        pagehtml = {page, curl.getSource(url)};

    /*int indexstart = StringHelper::GetNthIndex(pagehtml.second, "<article class=\"message-body", pagepart + 1);
    int indexend = pagehtml.second.find("</article>", indexstart)+(sizeof("</article>")-1);
    Console::Con() << "indexstart: " << indexstart << "\n";
    string relstring = pagehtml.second.substr(indexstart, indexend - indexstart);
    string newUrl = this->url + "#post" + relstring;
    Console::WriteLine("new url: " + newUrl);*/
    return this->url;
}
std::string SpaceBattlesGet::getCh(int ch)
{
    using std::vector, std::string, std::pair;
    CurlWrapper curl;
    StringHelper str;

    int page = (ch - 1) / 10;
    string site = this->url + (page == 0 ? "" : "page-" + std::to_string(page + 1));
    site = getChUrl(ch, site);
    string source = pagehtml.second;

    page = (ch - 1) / 10;
    int pagepart = (ch - 1) % 10;
    int chlistlenght = -1;
    if (chapterBuffer == pair<int, vector<string>>() || page != chapterBuffer.first)
    {
        auto chlist = StringHelper::GetAll(pagehtml.second, "<article class=\"message-body js-selectToQuote\">", "</article>");
        chlistlenght = chlist.size();
        Console::WriteLine("page htmlsecond_length: " + std::to_string(pagehtml.second.length()) + " - chlist length: " + std::to_string(chlist.size()));
        chapterBuffer = pair<int, vector<string>>(page, chlist);
    }
    if (chapterBuffer.second.size() <= pagepart)
    {
        Console::WriteLine( "chapterbuffer lenght error for chapter " + std::to_string(ch) + "\ndebug: page htmlsecondlength: " + std::to_string(pagehtml.second.length()) + " - chlist length: " + std::to_string(chlistlenght) );
        return "";
    }
    return chapterBuffer.second[pagepart];
}
int SpaceBattlesGet::GetChCount()
{
    if (chcount == -1)
    {
        using std::string;
        CurlWrapper curl;
        std::string sourcepage = curl.getSource(url);
        int latest = -1;
        int lastindex = sourcepage.find_last_of("<li class=\"pageNav-page");
        int endnumber = sourcepage.find("</a>", lastindex);
        string infostr = sourcepage.substr(lastindex, endnumber - lastindex);
        string relstr = infostr.substr(infostr.find_last_of(">"));
        StringHelper::replace(relstr, ">", "");
        latest = std::stoi(relstr) * 10;
        chcount = latest;
    }
    return chcount;
}
std::vector<std::string> SpaceBattlesGet::getAllCh(std::function<void(int, std::string)> callback, int delayinms)
{
    using std::vector, std::string;

    vector<string> allchapters;
    string ch;
    int chi = 1;
    while ((ch = getCh(chi)) != "")
    {
        allchapters.push_back(ch);
        callback(chi, ch);
        Console::WriteLine(chi);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayinms));
        chi++;
    }

    return allchapters;
}