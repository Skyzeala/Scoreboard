#pragma once
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
    //not yet implemented
    Underline, 
    Shadow,
    HeavyShadow
};

struct Color
{
    float r;
    float g;
    float b;
    Color(): r(0), g(0), b(0) {}
    Color(float r, float g, float b, float a = 1): r(r), g(g), b(b) {}
};

const int STRING_MAX = 30; //maximum characters to be stored and displayed, add 1 to arrays
const int GAME_MAX = 20; //maximum number of sets to store

void toString(int score, char * string);
void toInt(char * string, int &score);

#endif