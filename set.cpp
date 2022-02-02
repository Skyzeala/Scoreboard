#include "set.h"

void Set::incrementScore1() //limited to 99 in toString
{
    ++score1;
}
void Set::incrementScore2()
{
    ++score2;
}
void Set::decrementScore1()
{
    if (score1 > 0)
        --score1;
}
void Set::decrementScore2()
{
    if (score2 > 0)
        --score2;
}
void Set::resetScore()
{
    score1 = 0;
    score2 = 0;
}