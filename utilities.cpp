#include "utilities.h"




void toString(int score, char * string)
{
    string[0] = '0' + score / 10;
    string[1] = '0' + score % 10;
    string[2] = '\0';
}

void toInt(char * string, int &score)
{
    score = 0;
    int i = (int)string[0] - 48;
    if (i > 0 && i < 10)
        score = i;
    i = (int)string[1] - 48;
    if (i > 0 && i < 10)
    {
        score *= 10;
        score += i;
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