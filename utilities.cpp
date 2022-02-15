#include "utilities.h"



Color::Color() : 
    r(0), 
    g(0), 
    b(0) 
{
}

Color::Color(const Color &color) : 
    r(color.r), 
    g(color.g), 
    b(color.b) 
{
}

Color::Color(float r, float g, float b) : 
    r(r), 
    g(g), 
    b(b) 
{
}

Color::~Color()
{
    r = 0;
    g = 0;
    b = 0;
}

Color & Color::operator=(const Color &color)
{
    if (this == &color)
        return *this;
    r = color.r; 
    g = color.g; 
    b = color.b; 
    return *this;
}



void toString(int number, char * string)
{
    string[0] = '0' + number / 10;
    string[1] = '0' + number % 10;
    string[2] = '\0';
}

void toString(unsigned int number, char * string)
{
    string[0] = '0' + number / 10;
    string[1] = '0' + number % 10;
    string[2] = '\0';
}

void toInt(char * string, int &number)
{
    number = 0;
    int i = (int)string[0] - 48;
    if (i > 0 && i < 10)
        number = i;
    i = (int)string[1] - 48;
    if (i > 0 && i < 10)
    {
        number *= 10;
        number += i;
    }
}

void toInt(char * string, unsigned int &number)
{
    number = 0;
    int i = (int)string[0] - 48;
    if (i > 0 && i < 10)
        number = i;
    i = (int)string[1] - 48;
    if (i > 0 && i < 10)
    {
        number *= 10;
        number += i;
    }
}

char * strcpy(char * dest, char * src) //super basic, and now we dont need all of string.h
{
    for (int i = 0; i <= STRING_MAX + 1; ++i)
    {
        dest[i] = '\0';
    }
    for (int i = 0; i <= STRING_MAX && src[i] != '\0'; ++i)
    {
        dest[i] = src[i];
    }
    return dest;
}
