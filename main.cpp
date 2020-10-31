#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "tidywrapper.h"
#include "curlwrapper.h"
#include "stringhelper.h"
#include "royalroadget.h"
#include "console.h"
#include "zipwrapper.h"
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
        Console::WriteLine("no args enter url and chapter, example .\\main.exe https://www.royalroad.com/fiction/34473/shade-touched 1");
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
    std::ofstream myfile;
    myfile.open(std::to_string(mych)+".xhtml");
    myfile << content;
    myfile.close();
    return true;
}

int main(int argc, char *argv[]) 
{
    if(parseArgs(argc, argv)==false) return 1;
    //printf("test tidy");
    //std::cout << "Hello World!" << std::endl;
    //std::string url="https://www.royalroad.com/fiction/34473/shade-touched";
    //int ch=1;
    //std::string html;
    TidyWrapper td;
    //std::string xhtml = td.tidyhtmlToXHtml("<html><body><p>test</p><br><br><p>test2</p></body></html>");
    //std::cout << xhtml;

    RoyalRoadGet rrg(url);
    switch (currentjob)
    {
    case job::getall:
        rrg.getAllCh([](int chi, std::string ch){
            writeToFile(ch,chi);
        });
        break;
        case job::getch:
            std::cout << rrg.getCh(ch)<<std::endl;
        break;
    default:
        break;
    }
    
    //std::string xhtmlcontent = td.tidyhtmlToXHtml(chapter);
    //StringHelper::replace(xhtmlcontent, "<body>", "<body>\n<h2>Chapter "+std::to_string(ch)+"</h2><br />");
    //Console::WriteLine("---break---");
    //Console::WriteLine(xhtmlcontent);

    //writeToFile(xhtmlcontent);

    return 0;
}
