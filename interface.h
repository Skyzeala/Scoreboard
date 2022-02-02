#ifndef _SBINTERFACE_
#define _SBINTERFACE_

#include "utilities.h"

typedef void (*actionFunction)(void);

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
    actionFunction action;
    Button();
    Button(int leftx, int topy, int rightx, int bottomy, Color * color, ButtonPurpose purpose, actionFunction action);
    bool over(int x, int y);
};




#endif