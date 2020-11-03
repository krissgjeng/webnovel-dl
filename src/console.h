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
        //std::cout << 123 << std::endl;
        std::cout << line << std::endl;
    }
    static Console& Con() 
    {
        static Console theInstance;
        return theInstance;
    }
    /*std::ostream& operator<<(std::ostream& os)
{
    
}*/
template<typename T>
void operator<<( T& os)
{
    WriteLine(os);
}
private:

};
#endif // CONSOLE_H