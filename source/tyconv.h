#ifndef TYCONV_H
#define TYCONV_H

#include <string>
#include <sstream>

class tyconv
{
    public:
        template <class T>
        static std::string str(T str)
        {
            std::stringstream ss;
            ss << str;
            return ss.str();
        }

};

#endif // TYCONV_H
