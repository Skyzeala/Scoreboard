#include "interface.h"


bool onButton(int x, int y, Button &button)
{
    if (x > button.leftx && y > button.topy && x < button.rightx && y < button.bottomy)
        return true;
    return false;
}



