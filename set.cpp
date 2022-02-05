#include "set.h"

Set::Set() :
    setNumber(1), 
    score1(0),
    score2(0),
    team1Name("Home"),
    team2Name("Guest"),
    bgColor(Color(0.6,0.7,0.9)),
    textColor(Color(0.0,0.1,0)),
    accentColor(Color(0.8,0.8,0.8)),
    interactColor(Color(0.3,0.3,0.3)),
    team1Color(Color(0.8,0.2,0)),
    team2Color(Color(0,0.2,0.8))
{
}

Set::Set(Set &set) :
    setNumber(setNumber+1), 
    score1(0),
    score2(0),
    bgColor(bgColor),
    textColor(textColor),
    accentColor(accentColor),
    interactColor(interactColor),
    team1Color(team1Color),
    team2Color(team2Color)
{
    strcpy(this->team1Name, team1Name);
    strcpy(this->team2Name, team2Name);
}

Set::~Set()
{
}






void Set::incrementScore1()
{
    if (score1 < 99)
        ++score1;
}
void Set::incrementScore2()
{
    if (score2 < 99)
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



