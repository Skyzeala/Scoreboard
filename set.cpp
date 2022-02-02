#include "set.h"

Set::Set() :
    setNumber(1), 
    score1(0),
    score2(0),
    bgColor(Color(0.6,0.7,0.9)),
    textColor(Color(0.0,0.1,0)),
    accentColor(Color(0.8,0.8,0.8)),
    interactColor(Color(0.3,0.3,0.3)),
    team1Color(Color(0.8,0.2,0)),
    team2Color(Color(0,0.2,0.8))
{
    strcpy(team1Name, "Home");
    strcpy(team2Name, "Guest");

}



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



