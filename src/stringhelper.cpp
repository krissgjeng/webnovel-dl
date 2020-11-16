#include "stringhelper.h"
#include "console.h"

#include <algorithm>
#include <cctype>
#include <string>

extern "C"
{
    #include "external/htmlentities.h"
}

StringHelper::StringHelper()
{
}

std::string StringHelper::ToLower(const std::string& source)
{
    std::string data = source;
    std::transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
    return data; 
}

std::string StringHelper::Get(const std::string &sourceorg, const std::string &startstr, const std::string &endstr, bool inclusive, bool caseInsensitive)
{
    std::string source;
    if(caseInsensitive)
    {
        source = ToLower(sourceorg);
    }
    else
    {
         source = sourceorg;
    }
    
    
    int start = source.find(startstr);
    int end = source.find(endstr, start + startstr.length()) - start;
    //Console::Con() << "start: "<<start <<" to end: " << end << " for "+startstr+" to "+endstr+"\n";
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

int StringHelper::GetNthIndex(const std::string &source, const std::string &substring, int nth)
{
    int count = 0, n = 0;

    if (substring != "")
    {
        while ((n = source.find(substring, n)) != -1)
        {
            n += substring.length();
            ++count;
            if (count == nth)
                break;
        }
    }
    //Debug.WriteLine("getNthIndex: " + substring + "\nN: " + n + "\nCount: " + count);
    return n;
}
int StringHelper::CountStr(const std::string& source, const std::string& strtofind)
{
    int count = 0, n = 0;
    if (strtofind != "")
    {
        while ((n = source.find(strtofind, n)) != -1)
        {
            n += strtofind.length();
            count++;
        }
    }
    return count;
}
void StringHelper::RemoveIllegalCharsInFsName(std::string& source)
{
    using std::string;
    string illegalChars = "\\/:?\"<>|";
    for (auto it = source.begin() ; it < source.end() ; ++it){
        bool found = illegalChars.find(*it) != string::npos;
        if(found){
            *it = ' ';
        }
    }
}
std::string StringHelper::UnescapeHtmlEntities(const std::string& source)
{
    char buffer[32768]={};
    htmlentities::decode_html_entities_utf8(buffer,source.c_str());
    std::string decoded = buffer;
    return decoded;
}