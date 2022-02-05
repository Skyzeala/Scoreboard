#ifndef _SBSETDATA_
#define _SBSETDATA_

#include "utilities.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//currently unused, all pieces scattered around scoreboard.cpp

class Set
{
public:
    Set();
    Set(Set &set); //start new set based on previous set data like team name, colors, but reset the score 
    ~Set(); //no dynamic memory, so just a default deconstructor

    void incrementScore1(); //limited to 99 in toString
    void incrementScore2();
    void decrementScore1();
    void decrementScore2();
    void resetScore();

    void useBgColor();
    void useTextColor();
    void useAccentColor();
    void useHoverColor(int team); //calculates color and changes to it for the next draws
    void useClickColor(int team); //calculates color and changes to it for the next draws
    void useTeam1Color();
    void useTeam2Color();

    int getSetNumber();
    int getScore1();
    int getScore2();

    void getTeam1Name(char * name);
    void getTeam2Name(char * name);

    Color & getBgColor();
    Color & getTextColor();
    Color & getAccentColor();
    Color & getInteractColor();
    Color & getHoverColor(int team); //calculates color and returns it
    Color & getClickColor(int team); //calculates color and returns it
    Color & getTeam1Color();
    Color & getTeam2Color();

    void setBgColor();
    void setTextColor();
    void setAccentColor();
    void setInteractColor(); 
    void setTeam1Color();
    void setTeam2Color();

private:
    int setNumber;
    int score1;
    int score2;

    char team1Name[STRING_MAX + 1];
    char team2Name[STRING_MAX + 1];

    Color bgColor;
    Color textColor;
    Color accentColor;
    Color interactColor;
    Color team1Color;
    Color team2Color;

};

#endif