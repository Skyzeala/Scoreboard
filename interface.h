#ifndef _INTERFACE_
#define _INTERFACE_

#include "utilities.h"
#include <vector>


struct Button
{
    int leftx, topy, rightx, bottomy;
    Color * color;
    Button(): leftx(0), topy(0), rightx(0), bottomy(0), color(&bgColor) {}
    Button(int leftx, int topy, int rightx, int bottomy, Color * color): 
        leftx(leftx), topy(topy), rightx(rightx), bottomy(bottomy), color(color) {}
};

Button button = Button(100,100,100,400, &team1Color);
std::vector<Button> buttons;

#endif