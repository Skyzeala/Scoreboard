#ifndef _INTERFACE_
#define _INTERFACE_

#include "utilities.h"


enum ButtonPurpose
{
    DisplayOnly,
    Hoverable,
    Clickable,
    Editable
};

struct Button
{
    int leftx, topy, rightx, bottomy;
    Color * color;
    ButtonPurpose purpose;
    Button(): leftx(0), topy(0), rightx(0), bottomy(0), color(nullptr), purpose(DisplayOnly) {}
    Button(int leftx, int topy, int rightx, int bottomy, Color * color, ButtonPurpose purpose): 
        leftx(leftx), topy(topy), rightx(rightx), bottomy(bottomy), color(color), purpose(purpose) {}
};

bool onButton(int x, int y, Button &button);


#endif