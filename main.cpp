#include <iostream>
#include <string>
#include "tidywrapper.h"
//using https://code.visualstudio.com/docs/cpp/config-mingw
//
int main() {
    printf("test tidy");
    std::cout << "Hello World!" << std::endl;
    std::cout << "Hello World!" << std::endl;

    TidyWrapper td;
    std::string xhtml = td.tidyhtmlToXHtml("<html><body><p>test</p><br><br><p>test2</p></body></html>");

    std::cout << xhtml;
    /*
    int number;
    std::cin >> number;
    std::cout << number;*/
    return 0;
}