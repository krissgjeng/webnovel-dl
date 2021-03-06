#include "makeepub.h"
#include "royalroadget.h"
#include "external/tidywrapper.h"
#include "external/zipwrapper.h"
#include "console.h"
#include "stringhelper.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#define _GLIBCXX_HAS_GTHREADS
#include <chrono>
#include <thread>

MakeEpub::MakeEpub(NovelGet* ng, std::string outputdir)
{
    this->ng = ng;
    outDir = outputdir;
    workDir = outputdir+"/work";
    tmpDir = outputdir+"/tmp";
    std::filesystem::create_directories(workDir);
    std::filesystem::create_directory(tmpDir);
}
void MakeEpub::BuildEpub(std::string url)
{
    prepareStructure();
    downloadChapters(url);
    convertToXhtml();
    createToc();
    compressContent();
    cleanup();
}
void MakeEpub::BuildEpub(std::string url, int from, int to)
{
    prepareStructure();
    downloadChapters(url,from,to);
    convertToXhtml();
    createToc();
    compressContent();
    cleanup();
}

void MakeEpub::prepareStructure()
{
    std::ofstream mimetype;
    mimetype.open(workDir+"/mimetype");
    mimetype << "application/epub+zip";
    mimetype.close();

    std::filesystem::create_directory(workDir+"/META-INF");
    std::filesystem::create_directory(workDir+"/OEBPS");
}
void MakeEpub::downloadChapters(std::string url, int from, int to)
{
    int delayinms=750;
    chapters = ng->GetChCount();
    if(to==-1) to=chapters;
    chfrom = from; chto = to;
    chapters= chto - (from-1);
    Console::WriteLine("chps: "+std::to_string(chapters));

    ng->getChaptersCb(chfrom,chto,[this](int c, std::string chs){
        std::ofstream ch;
        if(c==1) chs ="<h1>"+ng->GetTitle()+"</h1>\n<h3>by "+ng->GetAuthor()+"</h3><h4>src: <a href=\""+ng->url+"\">"+ng->url+"</a></h4><h2>Chapter "+std::to_string(c)+"</h2><br />"+chs;
        else chs = "<h2>Chapter "+std::to_string(c)+"</h2><br />"+chs;
        ch.open( tmpDir+"/"+std::to_string(c)+".html" );
        htmlfiles.push_back(tmpDir+"/"+std::to_string(c)+".html");
        ch << chs;
        ch.close();
        Console::WriteLine("dl ch: "+std::to_string(c)+"/"+std::to_string(chto));
    },delayinms);
}
void MakeEpub::convertToXhtml()
{
    TidyWrapper td;
    for (size_t i = 0; i < htmlfiles.size(); i++)
    {
        std::ofstream chx;
        std::ifstream chh;
        chh.open(htmlfiles[i]);
        std::string fname = std::filesystem::path(htmlfiles[i]).stem().string()+".xhtml";
        chx.open(workDir+"/OEBPS/"+fname);
        std::stringstream contentss;
        contentss << chh.rdbuf();
        chh.close();
        std::string xhtml = td.tidyhtmlToXHtml(contentss.str());
        //if(i==0) StringHelper::replace(xhtml, "<body>", "<body>\n<h1>"+ng->GetTitle()+"</h1>\n<h3>by "+ng->GetAuthor()+"</h3><h4>src: <a href=\""+ng->url+"\">"+ng->url+"</a></h4><h2>Chapter "+std::to_string(i+chfrom)+"</h2><br />");
        //else StringHelper::replace(xhtml, "<body>", "<body>\n<h2>Chapter "+std::to_string(i+chfrom)+"</h2><br />");
        chx << xhtml;
        chx.close();
    }
}
void MakeEpub::createToc()
{
    using std::string, std::ofstream;

    string name=ng->GetTitle();
    string author=ng->GetAuthor();
    string publisher = "Royal Road";// "WebNovelReader";

    string containerxml = "<?xml version=\"1.0\"?>\r\n\
                           <container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\r\n\
                           \t<rootfiles>\r\n\
                           \t\t<rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>\r\n\
                           \r\n\t</rootfiles>\r\n\
                           </container>";
    
    ofstream container; container.open(workDir+"/META-INF/container.xml");
    container << containerxml;
    container.close();

    const auto time = std::chrono::system_clock::now();
    string uuid = "WebNovelReader"+std::to_string( std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count());
    string opfStart = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
            <package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >\n\
                <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n\
                    <dc:title>"+name+"</dc:title>\n\
                    <dc:creator opf:role=\"aut\">"+author+"</dc:creator>\n\
                    <dc:language>en-US</dc:language>\n\
                    <dc:rights>Public Domain</dc:rights>\n\
                    <dc:publisher>"+publisher+"</dc:publisher>\n\
                    <dc:identifier id=\"BookID\">"+uuid+"</dc:identifier>\n\
                </metadata>\n\
                <manifest>\n\
                    <item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />\n";
    string opfChapters;
    for (int ch = chfrom; ch <= chto; ++ch) {
        opfChapters+= "\t\t<item id=\"chapter"+std::to_string(ch)+"\" href=\""+std::to_string(ch)+".xhtml\" media-type=\"application/xhtml+xml\" />\r\n";
    }
    opfChapters+= "</manifest>\r\n\
            <spine toc=\"ncx\">\r\n";//<itemref idref=\"titlepage\" />\r\n";
    for (int ch = chfrom; ch <= chto; ++ch) {
        opfChapters+= "<itemref idref=\"chapter"+std::to_string(ch)+"\" />\r\n";
    }
    string opfEnd = "    </spine>\r\n</package>";
    string opfContent = opfStart+opfChapters+opfEnd;
    ofstream opffile; opffile.open(workDir+"/OEBPS/content.opf");
    opffile << opfContent;
    opffile.close();

    string ncxStart="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\
            <ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">\r\n\
            <head>\r\n\
                <meta name=\"dtb:uid\" content=\""+uuid+"\"/>\r\n\
                <meta name=\"dtb:depth\" content=\"1\"/>\r\n\
                <meta name=\"dtb:totalPageCount\" content=\"0\"/>\r\n\
                <meta name=\"dtb:maxPageNumber\" content=\"0\"/>\r\n\
            </head>\r\n\
            <docTitle>\r\n\
                <text>"+name+"</text>\r\n\
            </docTitle>\r\n\
            <navMap>";
    string ncxChapters;
    for (int ch = 0; ch < chapters; ++ch) {
        string chnr = std::to_string(ch+1);
        ncxChapters+="<navPoint id=\"chapter"+std::to_string(ch+chfrom)+"\" playOrder=\""+chnr+"\">\r\n\
                <navLabel>\r\n\
                    <text>Chapter "+std::to_string(ch+chfrom)+"</text>\r\n\
                </navLabel>\r\n\
                <content src=\""+std::to_string(ch+chfrom)+".xhtml\"/>\r\n\
            </navPoint>\r\n";
    }
    string ncxEnd="\t</navMap>\r\n</ncx>";
    string toc=ncxStart+ncxChapters+ncxEnd;
    ofstream tocncx; tocncx.open(workDir+"/OEBPS/toc.ncx");
    tocncx << toc;
    tocncx.close();
}
static std::string outfile="";
void MakeEpub::compressContent()
{
    using std::string;
    StringHelper str;
    string output = outDir+"/";
    string title =  str.UnescapeHtmlEntities(ng->GetTitle());
    string filename = title+"_"+std::to_string(chfrom)+"-"+std::to_string(chto)+".epub";
    StringHelper::RemoveIllegalCharsInFsName(filename);
    output = output+filename;
    outfile = output;
    ZipWrapper zw;

    zw.zip_append_file_store(workDir+"/mimetype","mimetype",output);
    std::filesystem::remove(workDir+"/mimetype");
    zw.zip_directory(workDir,output);
}
void MakeEpub::cleanup()
{
    if(std::filesystem::exists(outfile))
    {
        std::filesystem::remove_all(tmpDir);
        std::filesystem::remove_all(workDir);
    }
    htmlfiles.clear();
}
