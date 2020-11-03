#include "spacebattlesget.h"
#include "external/curlwrapper.h"
#include "stringhelper.h"
#include "console.h"
#include <vector>
#include <chrono>
#define _GLIBCXX_HAS_GTHREADS
#include <thread>

//not done
SpaceBattlesGet::SpaceBattlesGet(std::string url)
{
    this->url = url;
    //getChUrl(1); //inits stuff
}

std::string SpaceBattlesGet::getAuthor(const std::string& source)
{
    using std::string;
    StringHelper str;
    string authsrc = str.Get(source, "<h4", "</h4>");
    authsrc = str.Get(authsrc,"<a href=","/a>");
    authsrc = str.Get(authsrc,">","<");
    return authsrc.substr(1);
}
std::string SpaceBattlesGet::getTitle(const std::string& source)
{
    using std::string;
    StringHelper str;
    string titlesrc = str.Get(source, "<h1", "/h1>");
    return str.Get(titlesrc, ">","<").substr(1);
}

std::string SpaceBattlesGet::getChUrl(int ch, std::string url)
{
    using std::vector, std::string, std::pair;
    if (chapters.size() < 1)
    {
        CurlWrapper curl;
        StringHelper str;

        string source = curl.getSource(url);
        Author = getAuthor(source);
        Title = getTitle(source);

        int page = ( (ch-1) / 10);
        int pagepart = ( (ch-1) % 10);

        if (pagepart == 1)
        {
            if ( (pagehtml == pair<int,string>() || page != pagehtml.first) && page<=1)
                pagehtml = {page,  curl.getSource(url)};
            return this->url;
        }
        if (pagehtml == pair<int,string>() || page != pagehtml.first)
                pagehtml = {page,  curl.getSource(url)};

        /*int indexstart = StringHelper::GetNthIndex(pagehtml.second, "<li id=\"post" , pagepart+1);
        int indexend = pagehtml.second.find("\" ", indexstart);

        string relstring = pagehtml.second.mid(indexstart, indexend - indexstart);
        relstring = relstring.replace("<li id=\"", "");
        string newUrl = this->url + "#post" + relstring;
        Console::WriteLine( "new url: "+ newUrl );
        return newUrl;
    }
    if(ch-1<=chapters.size())
        return chapters[ch-1];
    else */
                }
        return "";
}
std::string SpaceBattlesGet::getCh(int ch)
{
    using std::vector, std::string;
    CurlWrapper curl;
    StringHelper str;
    
    int page = (ch-1)/10;
    string site = this->url + (page==0 ? "" : "/page-"+std::to_string(page+1));
    site = getChUrl(ch, site);
    string source = pagehtml.second;

    /* reform
    if(chapter<=0) chapter=currentProgress.currentChapter;
    int page = (chapter-1) / 10;
    int pagepart = (chapter-1) % 10;
    int chlistlenght=-1;
    if (chapterBuffer == QPair<int, QList<QString>>() || page != chapterBuffer.first)
    {
        auto chlist = StringHelper::GetAllInstances(pagehtml.second, "<article class=\"message-body js-selectToQuote\">", "</article>");
        chlistlenght=chlist.length();
        qDebug() << "page htmlsecond_length: " << pagehtml.second.length() << " - chlist length: " << chlist.length();
        chapterBuffer = QPair<int, QList<QString>>(page, chlist);
    }
    if(chapterBuffer.second.length()<=pagepart) return "chapterbuffer lenght error for chapter " +QString::number(chapter)+"\ndebug: page htmlsecondlength: " + QString::number(pagehtml.second.length()) + " - chlist length: " + QString::number(chlistlenght);
    return chapterBuffer.second[pagepart];
    */
}
int SpaceBattlesGet::GetChCount()
{
    getChUrl(1,this->url);
    return chapters.size();
}
std::vector<std::string> SpaceBattlesGet::getAllCh(std::function<void(int,std::string)> callback, int delayinms)
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