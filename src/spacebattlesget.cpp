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

    CurlWrapper curl;
    std::string source = curl.getSource(url);
    author = getAuthorSrc(source);
    title = getTitleSrc(source);
    Console::Con() << "spacebattles done\n";
}

std::string SpaceBattlesGet::getAuthorSrc(const std::string &source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "data-xf-init=\"member-tooltip\">", "</a>");
    str.replace(authsrc,"data-xf-init=\"member-tooltip\">","");
    return authsrc;
}
std::string SpaceBattlesGet::getTitleSrc(const std::string &source)
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
        string sourcepage = curl.getSource(url);
        int latest = -1;
        string findnav = "li class=\"pageNav-page";
        
        int lastindex = sourcepage.rfind(findnav)-20;
        string found = sourcepage.substr(lastindex);
        //Console::Con() << "Found: " << found <<"\n";
        int endnumber = sourcepage.find("</a>", lastindex);
        string infostr = sourcepage.substr(lastindex, endnumber - lastindex);
        //Console::Con() << "\nli:" << lastindex << " end: "<< endnumber << " max: " << sourcepage.size();
        string relstr = infostr.substr(infostr.find_last_of(">"));
        //Console::Con() << "counts:" << relstr;
        StringHelper::replace(relstr, ">", "");
        //latest = std::stoi(relstr) * 10;
        string lastpage = this->url + (std::stoi(relstr) > 0 ? "page-" + relstr : "");
        string lastsource = curl.getSource(lastpage);
        auto chlist = StringHelper::GetAll(lastsource, "<article class=\"message-body js-selectToQuote\">", "</article>");

        latest = ((std::stoi(relstr)-1) * 10) +chlist.size();

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