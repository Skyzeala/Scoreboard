#include "interface.h"






Button::Button(): 
    leftx(0), topy(0), rightx(0), bottomy(0), color(nullptr), purpose(DisplayOnly) 
{
}



Button::Button(int leftx, int topy, int rightx, int bottomy, Color * color, ButtonPurpose purpose, actionFunction action): 
    leftx(leftx), topy(topy), rightx(rightx), bottomy(bottomy), color(color), purpose(purpose), action(action)
{
}




bool Button::over(int x, int y)
{
    if (x > leftx && y > topy && x < rightx && y < bottomy)
        return true;
    return false;
}



