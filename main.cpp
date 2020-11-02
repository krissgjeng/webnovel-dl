#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include "tidywrapper.h"
#include "curlwrapper.h"
#include "stringhelper.h"
#include "royalroadget.h"
#include "console.h"
#include "zipwrapper.h"
#include "makeepub.h"
//using https://code.visualstudio.com/docs/cpp/config-mingw
//
std::string url="";
int ch=1;
enum job {none, getch, getall};
job currentjob=none;

bool parseArgs(int argc, char *argv[])
{
    if(argc<=1)
    {
        Console::WriteLine("no args, enter url and chapter, example .\\main.exe https://www.royalroad.com/fiction/34473/shade-touched 1");
        currentjob=job::none;
        return false;
    }
    /*std::cout << "You have entered " << argc-1
         << " arguments:" << "\n"; 
  
    for (int i = 1; i < argc; ++i) 
        std::cout << argv[i] << "\n";*/
        if(argc==3)
        {
            url = argv[1];
            ch = std::stoi(argv[2]);
            Console::WriteLine("url = "+url+" ch="+std::to_string(ch));
            currentjob = job::getch;
            return true;
        }
        else if(argc==2)
        {
            url = argv[1];
            Console::WriteLine("url = "+url+" ch=All");
            currentjob=job::getall;
            return true;
        }
        else return false;
}

bool writeToFile(std::string &content, int mych=-1)
{
    if(mych==-1) mych=ch;
    //std::string filepath= std::filesystem::current_path().string()+"/"+std::to_string(ch)+".xhtml";
    std::string fname = std::to_string(mych)+".xhtml";
    std::ofstream myfile;
    myfile.open(fname);
    myfile << content;
    myfile.close();
    
    return true;
}

int main(int argc, char *argv[]) 
{
    if(parseArgs(argc, argv)==false) return 1;
    //std::cout << "Hello World!" << std::endl;
    //std::string url="https://www.royalroad.com/fiction/34473/shade-touched";
    std::string chp="";
    RoyalRoadGet rrg(url);
    MakeEpub epub("epub");

    switch (currentjob)
    {
    case job::getall:
        //rrg.getAllCh([](int chi, std::string ch){
        //    writeToFile(ch,chi);

        //});
        epub.BuildEpub(url);
        break;
        case job::getch:
            chp = rrg.getCh(ch);
            //std::cout << chp <<std::endl;
            writeToFile(chp,ch);
        break;
    default:
        break;
    }
    
    //Console::WriteLine("---break---");
    //Console::WriteLine(xhtmlcontent);
    //writeToFile(xhtmlcontent);

    return 0;
}
