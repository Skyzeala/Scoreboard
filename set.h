#ifndef _SBSETDATA_
#define _SBSETDATA_

#include "utilities.h"

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



private:
    int setNumber;
    int score1;
    int score2;

    char team1Name[STRING_MAX + 1];
    char team2Name[STRING_MAX + 1];

    Color bgColor = Color(0.6,0.7,0.9);
    Color textColor = Color(0.0,0.1,0);
    Color accentColor = Color(0.8,0.8,0.8);
    Color interactColor = Color(0.3,0.3,0.3);
    Color team1Color = Color(0.8,0.2,0);
    Color team2Color = Color(0,0.2,0.8);


};

#endif