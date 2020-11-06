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

//using https://code.visualstudio.com/docs/cpp/config-mingw
//
std::string url = "";
int ch = 1;
enum job
{
    none,
    getch,
    getall
};
job currentjob = none;
NovelGet *ng = nullptr;

bool parseArgs(int argc, char *argv[])
{
    if (argc <= 1)
    {
        Console::WriteLine("no args, enter url and chapter, example .\\main.exe https://www.royalroad.com/fiction/34473/shade-touched 1");
        currentjob = job::none;
        return false;
    }
    url = argv[1];
    if (url.rfind("royalroad") !=std::string::npos)
    {
        Console::Con() << "is royalroad";
        ng = new RoyalRoadGet(url);
    }
    else if (url.rfind("spacebattles") !=std::string::npos)
    {
        Console::Con() << "is spacebattles";
        ng = new SpaceBattlesGet(url);
    }
    else
    {
        Console::Con() << "url: " << url << " is not supported";
        return false;
    }
    if (argc == 3)
    {
        ch = std::stoi(argv[2]);
        Console::WriteLine("url = " + url + " ch=" + std::to_string(ch));
        currentjob = job::getch;
        return true;
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
    Console::Con() << "chcount" << ng->GetChCount() << ", title: "<< ng->GetTitle() << ", author: "<<ng->GetAuthor()<<"\n";

    return 0;
    std::cout << "Hello World!" << std::endl;
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
    default:
        break;
    }

    //Console::WriteLine("---break---");
    Console::WriteLine("Done\n");
    //writeToFile(xhtmlcontent);
    delete ng;
    return 0;
}
