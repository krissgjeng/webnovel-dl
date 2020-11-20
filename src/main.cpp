#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include "external/tidywrapper.h"
#include "external/curlwrapper.h"
#include "stringhelper.h"
#include "console.h"
#include "external/zipwrapper.h"
#include "makeepub.h"

#include "novelget.h"
#include "royalroadget.h"
#include "spacebattlesget.h"
#include "fanfictionget.h"
#include "qqget.h"
#include "sufficientvelocityget.h"
#include "ao3get.h"

//using https://code.visualstudio.com/docs/cpp/config-mingw
//
std::string url = "";
int ch = 1;
int chfrom = -1;
int chto = -1;
enum job
{
    none,
    getch,
    getchps,
    getall
};
job currentjob = none;
NovelGet *ng = nullptr;

bool parseArgs(int argc, char *argv[])
{
    if (argc <= 1)
    {
        Console::Con() << "usage: main.exe [url] [ch], Downloads one chapter, example .\\main.exe https://www.royalroad.com/fiction/34473/shade-touched 1\n"
        << "usage 2: main.exe [url] [chapterrange], Downloads chapter range, example .\\main.exe https://www.fanfiction.net/s/12388283 2-30\n"
        << "usage 3: main.exe [url], Downloads all chapter to epub, example: main https://forums.spacebattles.com/threads/the-last-angel-ascension.346640/reader/\n";
        currentjob = job::none;
        return false;
    }
    url = argv[1];
    if (url.rfind("royalroad") != std::string::npos)
    {
        Console::Con() << "is royalroad\n";
        try { ng = new RoyalRoadGet(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
        
    }
    else if (url.rfind("spacebattles") != std::string::npos)
    {
        Console::Con() << "is spacebattles\n";
        try { ng = new SpaceBattlesGet(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
    }
    else if (url.rfind("sufficientvelocity") != std::string::npos)
    {
        Console::Con() << "is sufficientvelocity\n";
        try { ng = new SufficientVelocityGet(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
    }
    else if (url.rfind("fanfiction.net") != std::string::npos)
    {
        Console::Con() << "is fanfiction.net\n";
        try { ng = new FanFictionGet(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
    }
    else if (url.rfind("archiveofourown") != std::string::npos)
    {
        //https://archiveofourown.org/works/4833515
        Console::Con() << "is archiveofourown\n";
        try { ng = new AO3Get(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
    }
    else if (url.rfind("questionablequesting") != std::string::npos)
    {
        //url https://forum.questionablequesting.com/threads/with-this-ring-young-justice-si-story-only.8961/
        Console::Con() << "is questionablequesting\n";
        try { ng = new QuestionableQuestingGet(url); } catch (const std::exception& e) { Console::Con() << e.what(); return false; }
    }
    else
    {
        Console::Con() << "url: " << url << " is not supported\n";
        return false;
    }
    if (argc == 3)
    {
        std::string argchps = argv[2];
        int splitindex = -1;
        if ((splitindex = argchps.find("-")) != std::string::npos)
        {
            chfrom = std::stoi(argchps.substr(0, splitindex));
            chto = std::stoi(argchps.substr(splitindex + 1));
            Console::Con() << "url=" << url << " from ch: " << chfrom << " to " << chto << "\n";
            currentjob = job::getchps;
            return true;
        }
        else
        {
            ch = std::stoi(argv[2]);
            Console::WriteLine("url=" + url + " ch=" + std::to_string(ch));
            currentjob = job::getch;
            return true;
        }
    }
    else if (argc == 2)
    {
        Console::WriteLine("url = " + url + " ch=All");
        currentjob = job::getall;
        return true;
    }
    else
    {
        std::cout << "You have entered " << argc - 1
                  << " arguments:"
                  << "\n";

        for (int i = 1; i < argc; ++i)
            std::cout << argv[i] << "\n";

        return false;
    }
}

bool writeToFile(std::string &content, int mych = -1)
{
    if (mych == -1)
        mych = ch;
    //std::string filepath= std::filesystem::current_path().string()+"/"+std::to_string(ch)+".xhtml";
    std::string fname = std::to_string(mych) + ".xhtml";
    std::ofstream myfile;
    myfile.open(fname);
    myfile << content;
    myfile.close();

    return true;
}

int main(int argc, char *argv[])
{
    //Console::Con() << "test";
    if (parseArgs(argc, argv) == false)
        return 1;
    //Console::Con() << "chcount" << ng->GetChCount() << ", title: "<< ng->GetTitle() << ", author: "<<ng->GetAuthor()<<"\n";

    //return 0;
    //std::cout << "Hello World!" << std::endl;
    //std::string url="https://www.royalroad.com/fiction/34473/shade-touched";
    std::string chp = "";
    //RoyalRoadGet rrg(url);
    MakeEpub epub(ng, "epub");

    switch (currentjob)
    {
    case job::getall:
        //ng->getAllCh([](int chi, std::string ch) {
        //    writeToFile(ch, chi);
        //});
        epub.BuildEpub(url);
        break;
    case job::getch:
        chp = ng->getCh(ch);
        //std::cout << chp <<std::endl;
        writeToFile(chp, ch);
        break;
    case job::getchps:
        epub.BuildEpub(url, chfrom, chto);
        break;
    default:
        break;
    }

    //Console::WriteLine("---break---");
    Console::WriteLine("Done\n");
    //writeToFile(xhtmlcontent);
    delete ng;
    return 0;
}
