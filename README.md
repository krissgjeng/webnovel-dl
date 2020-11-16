# webnovel-dl
Downloads Webnovels and fanfictions and packages it to epubs

I have used this project to learn more about the build process of cpp and linking libraries.
my build process assumes you have 64bit clang++ in your PATH
just run compile.cmd or compile.sh, or run build task from vscode, compile.sh may be behind compile.cmd if i haven't tested linux in a while.

Currently supported websites with example of expected url:
fanfiction.net: https://www.fanfiction.net/s/12388283
royalroad.com: https://www.royalroad.com/fiction/34473/shade-touched
forums.spacebattles.com: https://forums.spacebattles.com/threads/the-last-angel-ascension.346640/reader/
forums.sufficientvelocity.com: https://forums.sufficientvelocity.com/threads/an-essence-of-silver-and-steel-worm-fate-stay-night-alt-power.39043/reader/
forums.questionablequesting.com: https://forum.questionablequesting.com/threads/with-this-ring-young-justice-si-story-only.8961/


Usage:
usage 1: main.exe [url] [ch], Downloads one chapter, example .\main.exe https://www.royalroad.com/fiction/34473/shade-touched 1
usage 2: main.exe [url] [chapterrange], Downloads chapter range, example .\main.exe https://www.fanfiction.net/s/12388283 2-30
usage 3: main.exe [url], Downloads all chapter to epub, example: .\main https://forums.spacebattles.com/threads/the-last-angel-ascension.346640/reader/


external libraries used:
libcurl with openssl
libzip
libtidy
