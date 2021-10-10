#ifndef _UTILITIES_
#define _UTILITIES_

enum ProgramState //this will define what is displayed on screen
{
    Active, //when the program is simply displaying normally and waiting for changes
    Settings, //when the settings page has been opened
    AwaitingInput //when a text box should be displayed
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
    float a;
    Color(): r(0), g(0), b(0), a(0) {}
    Color(float r, float g, float b, float a = 1): r(r), g(g), b(b), a(a) {}
};





void toString(int score, char * string);

#endif