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



void Set::useBgColor()
{
    glColor3f(bgColor.r, bgColor.g, bgColor.b);
}
void Set::useTextColor()
{
    glColor3f(textColor.r, textColor.g, textColor.b);
}
void Set::useAccentColor()
{
    glColor3f(accentColor.r, accentColor.g, accentColor.b);
}
void Set::useHoverColor(int team)
{
    if (team == 1)
        glColor3f((team1Color.r + interactColor.r)/2.0, (team1Color.g + interactColor.g)/2.0, (team1Color.b + interactColor.b)/2.0);
    else
        glColor3f((team2Color.r + interactColor.r)/2.0, (team2Color.g + interactColor.g)/2.0, (team2Color.b + interactColor.b)/2.0);
}
void Set::useClickColor(int team)
{
    if (team == 1)
        glColor3f((team1Color.r + interactColor.r*2)/3.0, (team1Color.g + interactColor.g*2)/3.0, (team1Color.b + interactColor.b*2)/3.0);
    else
        glColor3f((team2Color.r + interactColor.r*2)/3.0, (team2Color.g + interactColor.g*2)/3.0, (team2Color.b + interactColor.b*2)/3.0);
}
void Set::useTeam1Color()
{
    glColor3f(team1Color.r, team1Color.g, team1Color.b);
}
void Set::useTeam2Color()
{
    glColor3f(team2Color.r, team2Color.g, team2Color.b);
}



int Set::getSetNumber()
{
    return setNumber;
}
int Set::getScore1()
{
    return score1;
}
int Set::getScore2()
{
    return score2;
}

void Set::getTeam1Name(char * name)
{
    strcpy(name, team1Name);
}
void Set::getTeam2Name(char * name)
{
    strcpy(name, team2Name);
}

Color & Set::getBgColor()
{
    return bgColor;
}
Color & Set::getTextColor()
{
    return textColor;
}
Color & Set::getAccentColor()
{
    return accentColor;
}
Color & Set::getInteractColor()
{
    return interactColor;
}
Color & Set::getHoverColor(int team)
{
    Color color;
    if (team == 1)
    {
        color.r = (team1Color.r + interactColor.r)/2.0;
        color.g = (team1Color.g + interactColor.g)/2.0;
        color.b = (team1Color.b + interactColor.b)/2.0;
    }
    else
    {
        color.r = (team2Color.r + interactColor.r)/2.0;
        color.g = (team2Color.g + interactColor.g)/2.0;
        color.b = (team2Color.b + interactColor.b)/2.0;
    }
    return color;
}
Color & Set::getClickColor(int team)
{
    Color color;
    if (team == 1)
    {
        color.r = (team1Color.r + interactColor.r*2)/3.0;
        color.g = (team1Color.g + interactColor.g*2)/3.0;
        color.b = (team1Color.b + interactColor.b*2)/3.0;
    }
    else
    {
        color.r = (team2Color.r + interactColor.r*2)/3.0;
        color.g = (team2Color.g + interactColor.g*2)/3.0;
        color.b = (team2Color.b + interactColor.b*2)/3.0;
    }
    return color;
}
Color & Set::getTeam1Color()
{
    return team1Color;
}
Color & Set::getTeam2Color()
{
    return team2Color;
}

void Set::setBgColor()//oops, need color arg
{

}
void Set::setTextColor()
{

}
void Set::setAccentColor()
{

}
void Set::setInteractColor()
{

} 
void Set::setTeam1Color()
{

}
void Set::setTeam2Color()
{

}
