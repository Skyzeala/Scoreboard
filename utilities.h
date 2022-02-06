#ifndef _SBUTILITIES_
#define _SBUTILITIES_

enum ProgramState 
{
    HomeScreen,
    Settings,
    EditTeam1,
    EditTeam2,
    EditSetNum
};

enum FontStyle 
{
    Regular,
    Bold,
    Heavy,
    Italic, 
    BoldItalic,
    HeavyItalic,
    //below are not yet implemented
    Underline, 
    BoldUnderline,
    HeavyUnderline
};

struct Color
{
    float r;
    float g;
    float b;
    Color();
    Color(float r, float g, float b);
    ~Color();
    Color & operator=(const Color &color);
};

const int STRING_MAX = 30; //maximum characters to be stored and displayed, add 1 to arrays
const int GAME_MAX = 20; //maximum number of sets to store

const float shearMatrix [] = {1,0,0,0, //the matrix for the italics text transform
                        0.5,1,0,0,
                        0,0,1,0,
                        0,0,0,1};

void toString(int score, char * string);
void toInt(char * string, int &score);
char * strcpy(char * dest, char * src);

#endif