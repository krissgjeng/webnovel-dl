#include "stringhelper.h"
#include "console.h"

StringHelper::StringHelper()
{
}

std::string StringHelper::Get(std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive)
{
    int start = source.find(startstr);
    int end = source.find(endstr, start) - start;
    if (inclusive)
        end += endstr.length();
    return source.substr(start, end);
}
std::vector<std::string> StringHelper::GetAll(std::string &source, const std::string &startstr, const std::string &endstr, bool inclusive)
{
    using std::string, std::vector;
    vector<string> stringlist;
    int searchoffset = 0;
    //string newsource = source; //test if faster to reduce source instead of changing searchindex and scanning same source

    while (true)
    {
        //newsource =newsource.substr(searchoffset);
        int getfirstindex = source.find(startstr, searchoffset);
        //Console::WriteLine(std::string("firstind: ")+std::to_string(getfirstindex));
        //string fromfirst = source.substr(getfirstindex);
        int getendindex = source.find(endstr, getfirstindex) - getfirstindex;
        //Console::WriteLine(std::string("getendindex: ")+std::to_string(getendindex));
        if (getfirstindex <= -1)
            break;
        searchoffset = getfirstindex + getendindex; //+endstr.length();
        //Console::WriteLine(std::string("searchoffset: ")+std::to_string(searchoffset));
        //if(inclusive) getendindex+=endstr.length();
        string returnstring = source.substr(getfirstindex, getendindex);
        //if( (UniqueOnly && strlst.contains(returnstring))==false)
        stringlist.push_back(returnstring);
    }
    return stringlist;
}

void StringHelper::replaceAll(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
void StringHelper::replace(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    start_pos = str.find(from, start_pos);
    str.replace(start_pos, from.length(), to);
}