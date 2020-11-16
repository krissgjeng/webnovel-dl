#include "qqget.h"
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
QuestionableQuestingGet::QuestionableQuestingGet(std::string url)
{
    this->url = url;
    //getChUrl(1); //inits stuff

    CurlWrapper curl;
    std::string source = curl.getSource(url);
    author = getAuthorSrc(source);
    title = getTitleSrc(source);
    getPageLength(source);
}

std::string QuestionableQuestingGet::getAuthorSrc(const std::string &source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "data-author=", "\">");
    str.replace(authsrc,"data-author=\"","");
    return authsrc;
}
std::string QuestionableQuestingGet::getTitleSrc(const std::string &source)
{
    using std::string;
    StringHelper str;
    CurlWrapper cw;
    string titlesrc = str.Get(source, "<title>", "</title>");
    string title = str.Get(titlesrc, ">", " |").substr(1);
    title = cw.urlDecode(title);
    title = str.UnescapeHtmlEntities(title);
    return title;
}

std::string QuestionableQuestingGet::getChUrl(int ch, std::string url)
{
    using std::string, std::pair;
    CurlWrapper curl;
    StringHelper str;

    int page = ((ch - 1) / pageLength);
    int pagepart = ((ch - 1) % pageLength);
    if (pagepart == 1)
    {
        if ((pagehtml == pair<int, string>() || page != pagehtml.first) && page <= 1)
            pagehtml = {page, curl.getSource(url)};
        return this->url;
    }
    if (pagehtml == pair<int, string>() || page != pagehtml.first)
        pagehtml = {page, curl.getSource(url)};

    return this->url;
}
int QuestionableQuestingGet::getPageLength(std::string& source)
{
    if(pageLength>0)
        return pageLength;
    auto chlist = StringHelper::GetAll(source, "<article>", "</article>");
    pageLength = chlist.size();
    return pageLength;
}
std::string QuestionableQuestingGet::getCh(int ch)
{
    using std::vector, std::string, std::pair;
    CurlWrapper curl;
    StringHelper str;

    int page = (ch - 1) / pageLength;
    string site = this->url + (page == 0 ? "" : "page-" + std::to_string(page + 1));
    site = getChUrl(ch, site);
    string source = pagehtml.second;

    page = (ch - 1) / pageLength;
    int pagepart = (ch - 1) % pageLength;
    int chlistlenght = -1;
    if (chapterBuffer == pair<int, vector<string>>() || page != chapterBuffer.first)
    {
        auto chlist = StringHelper::GetAll(pagehtml.second, "<article>", "</article>");
        chlistlenght = chlist.size();
        Console::WriteLine("page htmlsecond_length: " + std::to_string(pagehtml.second.length()) + " - chlist length: " + std::to_string(chlist.size()));
        chapterBuffer = pair<int, vector<string>>(page, chlist);
        newUrlGet=true;
    }
    if (chapterBuffer.second.size() <= pagepart)
    {
        Console::WriteLine( "chapterbuffer lenght error for chapter " + std::to_string(ch) + "\ndebug: page htmlsecondlength: " + std::to_string(pagehtml.second.length()) + " - chlist length: " + std::to_string(chlistlenght) );
        return "";
    }
    return chapterBuffer.second[pagepart];
}
int QuestionableQuestingGet::GetChCount()
{
    if (chcount == -1)
    {
        using std::string;
        CurlWrapper curl;
        StringHelper str;
        string sourcepage = curl.getSource(url);
        int latest = -1;
        string findnav = str.Get(sourcepage,"<span class=\"pageNavHeader","</span>",true);
        findnav = str.Get(findnav,"Page 1 of","</span>");
        int pages = std::stoi(findnav.substr(10));

        string lastpage = this->url + (pages > 0 ? "page-" + std::to_string(pages) : "");
        string lastsource = curl.getSource(lastpage);
        auto chlist = StringHelper::GetAll(lastsource, "<article class=\"message-body js-selectToQuote\">", "</article>");

        latest = ((pages-1) * pageLength) +chlist.size();
        chcount = latest;
    }
    return chcount;
}
std::vector<std::string> QuestionableQuestingGet::getAllCh(std::function<void(int, std::string)> callback, int delayinms)
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
        if(newUrlGet)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayinms));
            newUrlGet=false;
        }
        chi++;
    }

    return allchapters;
}

void QuestionableQuestingGet::getAllCb(std::function<void(int, std::string)> callback, int delayinms)
{
    int to = GetChCount();
    getChaptersCb(1,to,callback,delayinms);
}
void QuestionableQuestingGet::getChaptersCb(int from, int to, std::function<void(int, std::string)> callback, int delayinms)
{
    using std::string;

    string ch;
    for (size_t i = from; i <= to; i++)
    {
        ch = getCh(i);
        callback(i, ch);
        if(newUrlGet)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayinms));
            newUrlGet=false;
        }
    }
}