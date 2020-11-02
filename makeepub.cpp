#include "makeepub.h"
#include "royalroadget.h"
#include "tidywrapper.h"
#include "zipwrapper.h"
#include "console.h"
#include "stringhelper.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#define _GLIBCXX_HAS_GTHREADS
#include <chrono>
#include <thread>


MakeEpub::MakeEpub(std::string outputdir)
{
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
    RoyalRoadGet rrg(url);
    int c = rrg.GetChCount();
    Console::WriteLine("chps: "+std::to_string(c));
    if(to==-1) to =c;
    chfrom = from; chto = to;
    chapters= chto - (from-1);

    for (size_t i = from; i <= to; i++)
    {
        std::ofstream ch;
        ch.open( tmpDir+"/"+std::to_string(i)+".html" );
        htmlfiles.push_back(tmpDir+"/"+std::to_string(i)+".html");
        ch << rrg.getCh(i);
        ch.close();
        Console::WriteLine("dl ch: "+std::to_string(i)+"/"+std::to_string(to));
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); //delay for preventing flooding of server and getting banned
    }
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
        StringHelper::replace(xhtml, "<body>", "<body>\n<h2>Chapter "+std::to_string(i+1)+"</h2><br />");
        chx << xhtml;
        chx.close();
    }
}
void MakeEpub::createToc()
{
    using std::string, std::ofstream;

    string name="royalorad";
    string author="royalorad";
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
                    <dc:publisher>WebNovelReader</dc:publisher>\n\
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
void MakeEpub::compressContent()
{
    using std::string;
    string output = outDir+"/output.epub";
    ZipWrapper zw;

    zw.zip_append_file_store(workDir+"/mimetype","mimetype",output);
    std::filesystem::remove(workDir+"/mimetype");
    zw.zip_directory(workDir,output);
}