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
    setNumber(setNumber), 
    score1(score1),
    score2(score1),
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
Set::Set(Set &set, int setNum) :
    setNumber(setNum), 
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

Set & Set::operator=(const Set &set)
{
    if (this == &set)
        return *this;
    setNumber = set.setNumber;
    score1 = set.score1;
    score2 = set.score2;

    strcpy(team1Name, set.team1Name);
    strcpy(team2Name, set.team2Name);

    bgColor = set.bgColor;
    textColor = set.textColor;
    accentColor = set.accentColor;
    interactColor = set.interactColor;
    team1Color = set.team1Color;
    team2Color = set.team2Color;
    return *this;
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
void Set::useHoverColor(Color &color)
{
    Color colorUse;

    colorUse.r = (color.r + interactColor.r)/2.0;
    colorUse.g = (color.g + interactColor.g)/2.0;
    colorUse.b = (color.b + interactColor.b)/2.0;
    
    glColor3f(colorUse.r, colorUse.g, colorUse.b);
}
void Set::useClickColor(Color &color)
{
    Color colorUse;

    colorUse.r = (color.r + interactColor.r*2)/3.0;
    colorUse.g = (color.g + interactColor.g*2)/3.0;
    colorUse.b = (color.b + interactColor.b*2)/3.0;
    
    glColor3f(colorUse.r, colorUse.g, colorUse.b);
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
Color & Set::getHoverColor(Color &color)
{
    Color colorRet;

    colorRet.r = (color.r + interactColor.r)/2.0;
    colorRet.g = (color.g + interactColor.g)/2.0;
    colorRet.b = (color.b + interactColor.b)/2.0;

    return colorRet;
}
Color & Set::getClickColor(Color &color)
{
    Color colorRet;

    colorRet.r = (color.r + interactColor.r*2)/3.0;
    colorRet.g = (color.g + interactColor.g*2)/3.0;
    colorRet.b = (color.b + interactColor.b*2)/3.0;
    
    return colorRet;
}
Color & Set::getTeam1Color()
{
    return team1Color;
}
Color & Set::getTeam2Color()
{
    return team2Color;
}

void Set::setSetNumber(int setNum)
{
    setNumber = setNum;
}
void Set::setScore1(int score)
{
    score1 = score;
}
void Set::setScore2(int score)
{
    score2 = score;
}

void Set::setTeam1Name(char * name)
{
    strcpy(team1Name, name);
}
void Set::setTeam2Name(char * name)
{
    strcpy(team2Name, name);
}



void Set::setBgColor(Color &color)//oops, need color arg
{
    bgColor = color;
}
void Set::setTextColor(Color &color)
{
    textColor = color;
}
void Set::setAccentColor(Color &color)
{
    accentColor = color;
}
void Set::setInteractColor(Color &color)
{
    interactColor = color;
} 
void Set::setTeam1Color(Color &color)
{
    team1Color = color;
}
void Set::setTeam2Color(Color &color)
{
    team2Color = color;
}
