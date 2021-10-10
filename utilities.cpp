#include "utilities.h"


void toString(int score, char * string)
{
    if (score > 99)
        score = 99;
    string[0] = '0' + score / 10;
    string[1] = '0' + score % 10;
    string[2] = '\0';
}


