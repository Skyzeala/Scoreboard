//things to do or think about
/*
glutFullScreen
game time, actual time
scores for each team and the set number
past set scores
glutPostRedisplay(); //after any visual variable change or sequence of changes
clear all button
glutGet(GLUT_SCREEN_WIDTH);

for the settings window glutCreateSubWindow(frameWin,FRAME_W,FRAME_H,canW,canH);
take a look at this https://stackoverflow.com/questions/22545934/glut-initialisation-function

glut documentation https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
*/

/*
TODO:
store multiple game scores
change team colors
make buttons positions depend on initial screen size or set positions, make it easier to move them in bulk

*/




/*
how set storage and navigation might work
resetScore sets both score values to 0, but does not change anything else about the set
discardSet removes the set from the list and the displayed set changes to the next lowest populated item in the list
    if no sets exist, create a new default set at set value 1 (index 0) and displays it
resetSets wipes all sets data and creates a default set at value 1 and displays it
saveSet pauses the timer, creates a new set with the same team names and colors at the next index if available, and displays the new set
loadSet loads the colors, team names, and score data of a selected set

need to do:
clarify the discard set button so that users are not easily confused about the function or purpose of it
make function names alight with button labels to avoid confusion for other programmers
*/

#include "utilities.h"
#include "interface.h"
#include "set.h"

#include <ctime>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define GLUT_BUILDING_LIB

using namespace std;

//initial window dimensions
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 500;

//OpenGL and GLUT setup functions
void displayCB();
void mouseCB(int button, int state, int mousex, int mousey);
void motionCB(int posx, int posy);
void keyboardCB(unsigned char key, int mousex, int mousey);
void timeCB(int value);
void reshapeCanvas(int width, int height);

void * font = GLUT_STROKE_MONO_ROMAN;
int mousePositionX;
int mousePositionY;
double mouseScaleX;
double mouseScaleY;
bool mousePressed;
vector<Button> homeScreenButtons;
vector<Button> editScreenButtons;

ProgramState programState = HomeScreen;

vector<Set> game;

Color accentColor;
Color team1Color;
Color team2Color;

char inputString[STRING_MAX + 1];
int textBoxIndex = 0;

unsigned int setNumber = 1;
unsigned int setToLoad = 1;

void init();

//visual program state screens
void displayHomeScreen();
void displaySettings();
void displayTextBox();

//drawing functions
void drawString(float posx, float posy, float size, char * str, FontStyle style);
void drawButton(float startx, float starty, float endx, float endy, Color &color);
void drawButton(Button &button);

//button action functions
//
void incrementScore1();
void incrementScore2();
void decrementScore1();
void decrementScore2();
void resetScore();
//

void stateToEditTeam1();
void stateToEditTeam2();
void stateToEditSetNum();
void discardInput();
void saveInput();
void saveSet();
void loadSet();
void discardSet();
void resetSets();

//init functions
void createButtons();


int main(int argc, char *argv[])
{
    mouseScaleX = 1;
    mouseScaleY = 1;
    
    init();

    glutInit(&argc, argv);          //initialize GLUT
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); //make a window
    glutInitWindowPosition(400,100);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    //glMatrixMode(GL_MODELVIEW);

    int scoreWindow = glutCreateWindow("Scoreboard"); //name the new window "Scoreboard"

    glutDisplayFunc(displayCB);	
    glutReshapeFunc(reshapeCanvas);
    glutMouseFunc(mouseCB);
    glutKeyboardFunc(keyboardCB);
    glutTimerFunc(1000, timeCB, 0);
    glutPassiveMotionFunc(motionCB);
    
    glutMainLoop();

    if (scoreWindow == 1)
        timeCB(1);

    return 0;
}


void displayCB()
{
    switch (programState)
    {
        case HomeScreen: displayHomeScreen();
            break;
        case Settings: displaySettings();
            break;
        default:
            displayTextBox();
    }
}

void displayHomeScreen()
{
    time_t currTime = time(0);
    struct tm timeStruct;
    char timeStr[STRING_MAX];
    char team1Name[STRING_MAX];
    char team2Name[STRING_MAX];
    char score1[STRING_MAX];
    char score2[STRING_MAX];
    char plus[STRING_MAX] = "+";
    char minus[STRING_MAX] = "-";
    char set[STRING_MAX];
    char saveSet[STRING_MAX] = "Save";
    char discardSet[STRING_MAX] = "Discard";
    game[setNumber - 1].getTeam1Name(team1Name);
    game[setNumber - 1].getTeam1Name(team2Name);
    toString(game[setNumber - 1].getScore1(), score1);
    toString(game[setNumber - 1].getScore2(), score2);
    toString(setNumber, set);
    timeStruct = *localtime(&currTime);
    strftime(timeStr, STRING_MAX, "%I:%M:%S", &timeStruct);

    game[setNumber - 1].useBgColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    for (unsigned int i = 0; i < homeScreenButtons.size(); i++)
    {
        drawButton(homeScreenButtons[i]);
    }

    game[setNumber - 1].useTextColor();
    drawString(280, 60, 50, timeStr, Bold);
    drawString(105, 132, 25, team1Name, BoldItalic);
    drawString(455, 132, 25, team2Name, BoldItalic);
    drawString(95, 340, 215, score1, Heavy);
    drawString(445, 340, 215, score2, Heavy);

    drawString(368, 215, 55, set, Bold);
    drawString(372, 275, 25, saveSet, Regular);
    drawString(364, 332, 17, discardSet, Regular);

    drawString(153, 387, 22, minus, Bold);
    drawString(281, 387, 22, plus, Bold);
    drawString(504, 387, 22, minus, Bold);
    drawString(632, 387, 22, plus, Bold);

    glutSwapBuffers();

}

void displaySettings()
{
    char str[STRING_MAX] = "not implemented";
    drawString(50,50, 20, str, Italic);
}


void displayTextBox()
{
    char underscore[STRING_MAX] = "_";
    char underscores[STRING_MAX] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
    char editing[STRING_MAX] = "Editing";
    char team[STRING_MAX] = "team";
    char setNum[STRING_MAX] = "set number";
    char save[STRING_MAX] = "Save";
    char discard[STRING_MAX] = "Discard";
    char team1Name[STRING_MAX];
    char team2Name[STRING_MAX];
    game[setNumber - 1].getTeam1Name(team1Name);
    game[setNumber - 1].getTeam2Name(team2Name);
    
    game[setNumber - 1].useBgColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    game[setNumber - 1].useTextColor();
    drawString(235, 50, 30, editing, Bold);
    if (programState == EditTeam1)
    {
        drawString(375, 50, 30, team, Bold);
        drawString(465, 50, 30, team1Name, Bold);
    }
    else if (programState == EditTeam2)
    {
        drawString(375, 50, 30, team, Bold);
        drawString(465, 50, 30, team2Name, Bold);
    }
    else if (programState == EditSetNum)
        drawString(375, 50, 30, setNum, Bold);

    //draw box for text to be entered into
    drawButton(15, 100, 785, 200, accentColor);

    //draw current input
    game[setNumber - 1].useTextColor();
    drawString(21, 165, 42, inputString, Bold);

    //text box underscores for user input spaces
    drawString(25, 180, 21, underscores, Regular);
    drawString(31, 180, 21, underscores, Regular);
    drawString(402, 180, 21, underscores, Regular);
    drawString(408, 180, 21, underscores, Regular);

    //highlight current index
    if (programState == EditTeam1)
        glColor3f(team1Color.r, team1Color.g, team1Color.b);
    else if (programState == EditTeam2)
        glColor3f(team2Color.r, team2Color.g, team2Color.b);
    drawString(25.13*(textBoxIndex + 1), 177, 33, underscore, Bold);

    //draw discard and save buttons
    for (unsigned int i = 0; i < editScreenButtons.size(); i++)
    {
        drawButton(editScreenButtons[i]);
    }
    game[setNumber - 1].useTextColor();
    drawString(125, 300, 40, discard, Bold);
    drawString(540, 300, 40, save, Bold);

    glutSwapBuffers();
}

//button: left middle or right, state: up or down, mousex and mousey: the cursor position when the state changes
//coded with help from https://stackoverflow.com/questions/27276075/glut-mouse-button-down
void mouseCB(int button, int state, int mousex, int mousey) //window relative coordinates
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mousePressed = true;
        for (unsigned int i = 0; (programState == HomeScreen) && i < homeScreenButtons.size(); i++)
        {
            if (homeScreenButtons[i].purpose == Clickable && homeScreenButtons[i].over(mousex * mouseScaleX, mousey * mouseScaleY))
            {
                homeScreenButtons[i].action();
                break; //in this program buttons will not overlap, so only one can be clicked at a time
            }
        }
        for (unsigned int i = 0; (programState > Settings) && i < editScreenButtons.size(); i++)
        {
            if (editScreenButtons[i].purpose == Clickable && editScreenButtons[i].over(mousex * mouseScaleX, mousey * mouseScaleY))
            {
                editScreenButtons[i].action();
                break;
            }
        }
    }
    else
        mousePressed = false;
    glutPostRedisplay();
}

void keyboardCB(unsigned char key, int mousex, int mousey) //window relative coordinates
{
    if (programState > Settings)
    {
        if (key == 27) //if escape is pressed, act like cancel
        {
            discardInput();
        }
        else if (key == 8 || key == 127) //if backspace or delete are pressed, remove the last character from the string
        {
            if (!(textBoxIndex == STRING_MAX - 1 && inputString[textBoxIndex] != '\0'))
                textBoxIndex -= 1;
            
            if (textBoxIndex < 0)
                textBoxIndex = 0;
              
            inputString[textBoxIndex] = '\0';
        }
        else if (key == 13) //if enter is pressed, act like save
        {
            saveInput();
        }
        else //add key to the input string when input is a normal character
        {
            inputString[textBoxIndex] = key;
            ++textBoxIndex;
        }
    }

    if (textBoxIndex >= STRING_MAX) //ensures the string is always null terminated and never overflowed
        textBoxIndex = STRING_MAX - 1;

    glutPostRedisplay();
}

void timeCB(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000, timeCB, 0);
}

void motionCB(int posx, int posy)
{
    mousePositionX = posx;
    mousePositionY = posy;
    glutPostRedisplay();
}

void drawButton(float startx, float starty, float endx, float endy, Color &color)
{
    glLoadIdentity();
    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glTranslatef(-1,1,0);
    glScalef(2.0f/(WINDOW_WIDTH), -2.0f/(WINDOW_HEIGHT), 1.0f);
    glBegin(GL_POLYGON);
		glVertex2f(startx,starty);
		glVertex2f(endx,starty);
        glVertex2f(endx,endy);
        glVertex2f(startx,endy);
	glEnd();
    glPopMatrix();
}

void drawButton(Button &button)
{
    Color color;

    glLoadIdentity();
    glPushMatrix();
    //change button color when hovered over or clicked on
    if (button.over(mousePositionX * mouseScaleX, mousePositionY * mouseScaleY) && (button.purpose == Hoverable || button.purpose == Clickable))
    {
        if (button.purpose == Clickable && mousePressed) //if button is clicked
            game[setNumber - 1].useClickColor(*(button.color));
        else //if button is hovered
            game[setNumber - 1].useHoverColor(*(button.color));
    }
    else //use regular button color when not clicked or hovered
        glColor3f(button.color->r, button.color->g, button.color->b);
    
    glTranslatef(-1,1,0);
    glScalef(2.0f/(WINDOW_WIDTH), -2.0f/(WINDOW_HEIGHT), 1.0f);
    glBegin(GL_POLYGON);
		glVertex2f(button.leftx,button.topy);
		glVertex2f(button.rightx,button.topy);
        glVertex2f(button.rightx,button.bottomy);
        glVertex2f(button.leftx,button.bottomy);
	glEnd();
    glPopMatrix();
}


void drawString(float posx, float posy, float size, char * str, FontStyle style)
{
    for (double i = 0; i <= style%3; i+=0.2)
    {
        for (double j = 0; j <= style%3; j+=0.2)
        {
            glPushMatrix();
            
            glTranslatef((posx + i - WINDOW_WIDTH/2.0)/(WINDOW_WIDTH/2.0), (WINDOW_HEIGHT/2.0 - posy - j)/(WINDOW_HEIGHT/2.0), 0.0f);
            glScalef(size/70000.0f, size/31000.0f, 1.0f);
            if (style >= Italic && style < Underline)
                glMultMatrixf(shearMatrix);
            for (int i = 0; i < STRING_MAX && str[i] != '\0'; i++) 
            {
                glutStrokeCharacter(font, str[i]);
            }
            glPopMatrix();
        }
    }
}


void reshapeCanvas(int w, int h)
{
    mouseScaleX = WINDOW_WIDTH * 1.0 / w;
    mouseScaleY = WINDOW_HEIGHT * 1.0 / h;

    glViewport(0, 0, w, h);
}

void init()
{
    loadSet();
    createButtons();
}

void createButtons()
{
    //team names panels
    homeScreenButtons.push_back(Button(100, 100, 350, 140, &accentColor, Clickable, stateToEditTeam1));
    homeScreenButtons.push_back(Button(450, 100, 700, 140, &accentColor, Clickable, stateToEditTeam2));

    //main score display panels
    homeScreenButtons.push_back(Button(100, 150, 220, 350, &team1Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(230, 150, 350, 350, &team1Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(450, 150, 570, 350, &team2Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(580, 150, 700, 350, &team2Color, DisplayOnly, nullptr));

    //add and subtract from score buttons
    homeScreenButtons.push_back(Button(100, 360, 220, 400, &accentColor, Clickable, decrementScore1));
    homeScreenButtons.push_back(Button(230, 360, 350, 400, &accentColor, Clickable, incrementScore1));
    homeScreenButtons.push_back(Button(450, 360, 570, 400, &accentColor, Clickable, decrementScore2));
    homeScreenButtons.push_back(Button(580, 360, 700, 400, &accentColor, Clickable, incrementScore2));

    //set display and set options
    homeScreenButtons.push_back(Button(360, 150, 440, 230, &accentColor, Clickable, stateToEditSetNum));
    homeScreenButtons.push_back(Button(360, 240, 440, 290, &accentColor, Clickable, saveSet));
    homeScreenButtons.push_back(Button(360, 300, 440, 350, &accentColor, Clickable, discardSet));

    //text editing buttons
    editScreenButtons.push_back(Button(70, 240, 350, 330, &accentColor, Clickable, discardInput));
    editScreenButtons.push_back(Button(450, 240, 730, 330, &accentColor, Clickable, saveInput));
}



void stateToEditTeam1()
{
    programState = EditTeam1;
}
void stateToEditTeam2()
{
    programState = EditTeam2;
}
void stateToEditSetNum()
{
    programState = EditSetNum;
}
void stateToHomeScreen()
{
    programState = HomeScreen;
}
void saveInput()
{
    switch(programState)
    {
        case EditTeam1:
            game[setNumber - 1].setTeam1Name(inputString);
            break;
        case EditTeam2:
            game[setNumber - 1].setTeam2Name(inputString);
            break;
        case EditSetNum:
            toInt(inputString, setNumber);
            if (setNumber > GAME_MAX || setNumber < 1)
            {
                setNumber = 1;
                game[setNumber - 1].setSetNumber(setNumber);
            }
            break;
        default:
            break;
    }
    discardInput(); //clean up input messes and set program state back to home
}
void discardInput()
{
    //clean up from any input
    for (int i = 0; i < STRING_MAX + 1; ++i)
    {
        inputString[i] = '\0';
    }
    textBoxIndex = 0;
    //go back to home state
    stateToHomeScreen();
}
void saveSet()
{
    //todo
    //put score1, score2, and timer time into an object in a list at index set - 1
    //maybe also store team colors and names

}
void loadSet()
{
    //todo
    //extract scores and timer (maybe also colors) and display them as if it were current
    if (setToLoad > game.size() + 1)
    {
        setToLoad = game.size();
        game.push_back(Set());
    }
    else if (setToLoad == game.size() + 1)
    {
        game.push_back(Set(game[setToLoad - 1], setToLoad));
    }



    setNumber = game[setToLoad - 1].getSetNumber();
    if (setNumber != setToLoad)
    {
        setNumber = setToLoad;
        game[setToLoad - 1].setSetNumber(setNumber);
    }
    
    accentColor = game[setToLoad - 1].getAccentColor();
    team1Color = game[setToLoad - 1].getTeam1Color();
    team2Color = game[setToLoad - 1].getTeam2Color();
}
void discardSet()
{
    //todo
    //check if current index (set number) is occupied and remove it
    resetScore();
} 







