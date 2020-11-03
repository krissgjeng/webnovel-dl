#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>
#include <iostream>

class Console
{
public:
    Console();
    template<typename T>
    static void WriteLine(T line)
    {
        //std::cout << 123 << std::endl;
        std::cout << line << std::endl;
    }
private:

};
#endif // CONSOLE_H