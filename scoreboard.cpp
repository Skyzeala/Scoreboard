//things to do or think about
/*
glutFullScreen
game time, actual time
scores for each team and the set number
past set scores
glutPostRedisplay(); //after any visual variable change or sequence of changes
clear all button
glutGet(GLUT_SCREEN_WIDTH);
button types, click for an action, hover, enter user input, do nothing
for the settings window glutCreateSubWindow(frameWin,FRAME_W,FRAME_H,canW,canH);
take a look at this https://stackoverflow.com/questions/22545934/glut-initialisation-function
glutCreateSubWindow(frameWin,FRAME_W, glutGet(GLUT_WINDOW_HEIGHT)-RUL_H, glutGet(GLUT_WINDOW_WIDTH)-FRAME_W, RUL_H);
for mouseover color changes void glutMotionFunc(void (*func)(int x, int y));
void glutPassiveMotionFunc(void (*func)(int x, int y));
glut can render fonts
glUseProgram makes subsequent draws use a particular shader program

glut documentation https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
*/

#include "utilities.h"
#include "interface.h"

#include <iostream>
#include <ctime>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define GLUT_BUILDING_LIB
//#define GLUT_DISABLE_ATEXIT_HACK

using namespace std;

#define STRING_MAX 30
#define GAME_MAX 10
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500

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
vector<Button> buttons;

Color bgColor = Color(0.6,0.7,0.9);
Color textColor = Color(0.0,0.4,0);
Color accent1Color = Color(0.8,0.8,0.8);
Color accent2Color = Color(0.2,0.2,0.2);
Color team1Color = Color(0.8,0.2,0);
Color team2Color = Color(0,0.2,0.8);

int score1 = 0;
int score2 = 0;


//drawing functions
void drawString(float posx, float posy, float size, char * str, FontStyle style);
void drawButton(float startx, float starty, float endx, float endy, Color &color);
void drawButton(Button &button);

//button action functions
void incrementScore1();
void incrementScore2();
void decrementScore1();
void decrementScore2();

//init functions
void createButtons();

float shearMatrix [] = {1,0,0,0, //the matrix for the italics text transform
                        0.5,1,0,0,
                        0,0,1,0,
                        0,0,0,1};

int main(int argc, char *argv[])
{
    createButtons();

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
    //glutReshapeFunc(reshapeCanvas);
    glutMouseFunc(mouseCB);
    //glutKeyboardFunc(keyboardCB);
    glutTimerFunc(1000, timeCB, 0);
    glutPassiveMotionFunc(motionCB);
    
    glutMainLoop();

    if (scoreWindow == 1)
        timeCB(1);

    return 0;
}


void displayCB()
{
    time_t currTime = time(0);
    struct tm timeStruct;
    char timeStr[STRING_MAX];
    char scoreA[STRING_MAX];
    char scoreB[STRING_MAX];
    char plus[STRING_MAX] = "+";
    char minus[STRING_MAX] = "-";
    toString(score1, scoreA);
    toString(score2, scoreB);
    timeStruct = *localtime(&currTime);
    strftime(timeStr, STRING_MAX, "%I:%M:%S", &timeStruct);

    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        drawButton(buttons[i]);
    }

    std::cout << timeStr << std::endl;

    glColor3f(textColor.r, textColor.g, textColor.b);
    drawString(270, 80, 60, timeStr, Bold);
    drawString(100, 340, 220, scoreA, Heavy);
    drawString(450, 340, 230, scoreB, Heavy);

    drawString(150, 397, 20, minus, Bold);
    drawString(280, 397, 20, plus, Bold);
    drawString(505, 397, 20, minus, Bold);
    drawString(630, 397, 20, plus, Bold);

    glutSwapBuffers();

}

//button: left middle or right, state: up or down, mousex and mousey: the cursor position when the state changes
//coded with help from https://stackoverflow.com/questions/27276075/glut-mouse-button-down
void mouseCB(int button, int state, int mousex, int mousey) //window relative coordinates
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        for (unsigned int i = 0; i < buttons.size(); i++)
        {
            if (buttons[i].over(mousex, mousey))
            {
                buttons[i].action();
                break; //in this program buttons will not overlap, so only one can be clicked at a time
            }
        }
    }
}

void keyboardCB(unsigned char key, int mousex, int mousey) //window relative coordinates
{
    

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
    glLoadIdentity();
    glPushMatrix();
    //change button color when hovered over
    if (button.over(mousePositionX, mousePositionY) && (button.purpose == Hoverable || button.purpose == Clickable))
        glColor3f((button.color->r + accent2Color.r)/2.0, (button.color->g + accent2Color.g)/2.0, (button.color->b + accent2Color.b)/2.0);
    else
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

void drawTextBox()
{
    
}

void drawString(float posx, float posy, float size, char * str, FontStyle style)
{
    for (int i = 0; i <= style%3; i++)
    {
        for (int j = 0; j <= style%3; j++)
        {
            glPushMatrix();
            
            glTranslatef((posx + i - WINDOW_WIDTH/2.0)/(WINDOW_WIDTH/2.0), (WINDOW_HEIGHT/2.0 - posy - j)/(WINDOW_HEIGHT/2.0), 0.0f);
            glScalef(size/80000.0f, size/31000.0f, 1.0f);
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

//this code is from http://www.lighthouse3d.com/tutorials/glut-tutorial/preparing-the-window-for-a-reshape/
void reshapeCanvas(int w, int h)
{
    // Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}



void createButtons()
{
    //main score display panels
    buttons.push_back(Button(100, 150, 220, 350, &team1Color, Hoverable, nullptr));
    buttons.push_back(Button(230, 150, 350, 350, &team1Color, Hoverable, nullptr));
    buttons.push_back(Button(450, 150, 570, 350, &team2Color, Hoverable, nullptr));
    buttons.push_back(Button(580, 150, 700, 350, &team2Color, Hoverable, nullptr));

    //add and subtract from score buttons
    buttons.push_back(Button(100, 370, 220, 410, &accent1Color, Clickable, decrementScore1));
    buttons.push_back(Button(230, 370, 350, 410, &accent1Color, Clickable, incrementScore1));
    buttons.push_back(Button(450, 370, 570, 410, &accent1Color, Clickable, decrementScore2));
    buttons.push_back(Button(580, 370, 700, 410, &accent1Color, Clickable, incrementScore2));
}



void incrementScore1()
{
    ++score1;
}
void incrementScore2()
{
    ++score2;
}
void decrementScore1()
{
    if (score1 > 0)
        --score1;
}
void decrementScore2()
{
    if (score2 > 0)
        --score2;
}











