#include "utilities.h"


class Set
{
public:
    Set();
    Set(Set &set);
    ~Set();

    void incrementScore1(); //limited to 99 in toString
    void incrementScore2();
    void decrementScore1();
    void decrementScore2();
    void resetScore();



private:
    Color bgColor = Color(0.6,0.7,0.9);
    Color textColor = Color(0.0,0.1,0);
    Color accentColor = Color(0.8,0.8,0.8);
    Color interactColor = Color(0.3,0.3,0.3);
    Color team1Color = Color(0.8,0.2,0);
    Color team2Color = Color(0,0.2,0.8);

    char team1Name[STRING_MAX + 1] = "Home";
    char team2Name[STRING_MAX + 1] = "Guest";

    int setNumber;
    int score1;
    int score2;


};