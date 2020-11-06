#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>
#include <iostream>

class Console
{
public:
    Console();
    template<typename T>
    static void WriteLine(const T& line)
    {
        std::cout << line << std::endl;
    }
    static Console& Con() 
    {
        static Console theInstance;
        return theInstance;
    }
    template <typename T>
    friend Console& operator<<(Console& log, T const& value);

private:

};
template <typename T>
Console& operator<<(Console& log, T const& value)
{
    std::cout << value;
    return log;
}

#endif // CONSOLE_H