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
    Set(const Set &set);
    Set(const Set &set, int setNum); //create new set for the same teams, reset the score 
    ~Set(); //no dynamic memory, so just a default deconstructor
    Set & operator=(const Set &set);

    void incrementScore1(); //limited to 99 in toString
    void incrementScore2();
    void decrementScore1();
    void decrementScore2();
    void resetScore();

    void useBgColor();
    void useTextColor();
    void useAccentColor();
    void useHoverColor(Color &color); //calculates color and changes to it for the next draws
    void useClickColor(Color &color); //calculates color and changes to it for the next draws
    void useTeam1Color();
    void useTeam2Color();

    int getSetNumber();
    int getScore1();
    int getScore2();

    void getTeam1Name(char * name);
    void getTeam2Name(char * name);

    //let gcc optimize the return, modern compilers know how to make returning an object efficient
    Color getBgColor();
    Color getTextColor();
    Color getAccentColor();
    Color getInteractColor();
    Color getHoverColor(Color &color); //calculates color and returns it
    Color getClickColor(Color &color); //calculates color and returns it
    Color getTeam1Color();
    Color getTeam2Color();

    void setSetNumber(int setNum);
    void setScore1(int score);
    void setScore2(int score);

    void setTeam1Name(char * name);
    void setTeam2Name(char * name);

    void setBgColor(Color &color);
    void setTextColor(Color &color);
    void setAccentColor(Color &color);
    void setInteractColor(Color &color); 
    void setTeam1Color(Color &color);
    void setTeam2Color(Color &color);

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