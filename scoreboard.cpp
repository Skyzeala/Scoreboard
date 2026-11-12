//0,0 is top left corner





//things to do or think about
/*
    game time, pause, play
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
    custom team colors
    texture map ball
    ball physics
    refactor code
    new set, store set, set saving conflict management

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
    make function names align with button labels to avoid confusion for other programmers
*/

#define _USE_MATH_DEFINES
#define GLUT_BUILDING_LIB
#define GL_GLEXT_PROTOTYPES

#include "utilities.h"
#include "interface.h"
#include "set.h"

#include <ctime>
#include <vector>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



using namespace std;

//initial window dimensions and reference coordinates
int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;

//create a grid for easily lining up elements on screen
const int columns = 18;
const int rows = 18;
int column[columns+1];
int row[rows+1];
int margin = WINDOW_WIDTH/160;

bool fullScreen = false;

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

const int sphereHeightDetail = 20; //number of vertices between top and bottom of sphere
const int sphereRotationalDetail = 40; //number of vertices around the sides
const int numberOfTriangles = sphereHeightDetail*sphereRotationalDetail*2;
const int numberOfVertices = 3*numberOfTriangles;
const int numberOfCoordinates = 3*numberOfVertices;
GLfloat sphereVertices[numberOfCoordinates]; //x and y and z coordinates for each triangle point
GLfloat sphereTextureCoordinates[numberOfCoordinates];
unsigned int ballTexture;
int ballRotX = 0;
int ballRotY = 0;
int ballRotZ = 0;
int ballRotVelocityX = 0;
int ballRotVelocityY = 0;
int ballRotVelocityZ = 0;

//initialize the scoreboard for first use, set up the first Set object
void init();
//init functions
void createButtons();
void createSphere();
void initColumnsAndRows();

//visual program state screens
void displayHomeScreen();
void displaySettings();
void displayTextBox();

//drawing functions
void drawString(float posx, float posy, float size, char * str, FontStyle style);
void drawButton(float startx, float starty, float endx, float endy, Color &color);
void drawButton(Button &button);
void drawGrid();
void drawBall(int x, int y, float size, float rotx, float roty, float rotz, int texture);

//button action functions, calls set functions
void incrementScore1();
void incrementScore2();
void decrementScore1();
void decrementScore2();
void resetScore();

//state manipulation of the program
void stateToEditTeam1();
void stateToEditTeam2();
void stateToEditSetNum();
void discardInput();
void saveInput();
void saveSet();
void loadSet();
void discardSet();
void resetSets();

void fullScreenProgram();
void windowProgram();


int main(int argc, char *argv[])
{
    mouseScaleX = 1;
    mouseScaleY = 1;
    
    glutInit(&argc, argv);          //initialize GLUT
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); //make a window
    glutInitWindowPosition(300,100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

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
    
    init();
    
    glutMainLoop();

    if (scoreWindow == 1)
        timeCB(1);

    return 0;
}


void displayCB()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
    char fullscreen[STRING_MAX];
    if (fullScreen)
        strcpy(fullscreen,"] [");
    else
        strcpy(fullscreen,"[ ]");
    char settings[STRING_MAX] = "*";
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
    game[setNumber - 1].getTeam2Name(team2Name);
    toString(game[setNumber - 1].getScore1(), score1);
    toString(game[setNumber - 1].getScore2(), score2);
    toString(setNumber, set);
    timeStruct = *localtime(&currTime);
    strftime(timeStr, STRING_MAX, "%I:%M:%S", &timeStruct);

    game[setNumber - 1].paintBg();

    //drawGrid();

    for (unsigned int i = 0; i < homeScreenButtons.size(); i++)
    {
        drawButton(homeScreenButtons[i]);
    }

    game[setNumber - 1].useTextColor();


    drawString(column[0]+margin*2-1, row[1]+margin+2, 20, fullscreen, Regular);
    drawString(column[7]-margin*3/2, row[2]-margin*2, 40, timeStr, Bold);
    drawString(column[17]+2, row[2]-margin*2+2, 56, settings, Bold);

    drawString(column[2]+margin, row[5]-margin*3, 25, team1Name, BoldItalic);
    drawString(column[10]+margin, row[5]-margin*3, 25, team2Name, BoldItalic);
    drawString(column[2], row[12]+margin, 222, score1, Heavy);
    drawString(column[10], row[12]+margin, 222, score2, Heavy);

    drawString(column[8]+margin*2, row[8], 60, set, Bold);
    drawString(column[8]+margin*3, row[11]-margin*3, 25, saveSet, Regular);
    drawString(column[8]+margin*2, row[13]-margin*4, 17, discardSet, Regular);

    drawString(column[3]+margin*3, row[14]+margin*2-2, 25, minus, Bold);
    drawString(column[6]+margin*3, row[14]+margin*2-2, 25, plus, Bold);
    drawString(column[11]+margin*3, row[14]+margin*2-2, 25, minus, Bold);
    drawString(column[14]+margin*3, row[14]+margin*2-2, 25, plus, Bold);
    
    drawBall(column[9], row[5]-margin*2.65, 165, ballRotX, ballRotY, ballRotZ, ballTexture);

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
    
    game[setNumber - 1].paintBg();

    game[setNumber - 1].useTextColor();
    drawString(190, 50, 30, editing, Bold);
    if (programState == EditTeam1)
    {
        drawString(360, 50, 30, team, Bold);
        drawString(465, 50, 30, team1Name, Bold);
    }
    else if (programState == EditTeam2)
    {
        drawString(360, 50, 30, team, Bold);
        drawString(465, 50, 30, team2Name, Bold);
    }
    else if (programState == EditSetNum)
        drawString(360, 50, 30, setNum, Bold);

    //draw box for text to be entered into
    drawButton(15, 100, 785, 200, accentColor);

    //draw current input
    game[setNumber - 1].useTextColor();
    drawString(21, 165, 42, inputString, Bold);

    //text box underscores for user input spaces
    drawString(27, 177, 21, underscores, Regular);
    drawString(35, 177, 21, underscores, Regular);
    drawString(331, 177, 21, underscores, Regular);
    drawString(338, 177, 21, underscores, Regular);

    //highlight current index
    if (programState == EditTeam1)
        glColor3f(team1Color.r, team1Color.g, team1Color.b);
    else if (programState == EditTeam2)
        glColor3f(team2Color.r, team2Color.g, team2Color.b);
    drawString(30.2*(textBoxIndex + 1)-6, 172, 36, underscore, Bold);

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
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
    
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
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

void drawGrid()
{
    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.95, 0.95, 0.3);
    glTranslatef(-1,1,0);
    glScalef(2.0f/(WINDOW_WIDTH), -2.0f/(WINDOW_HEIGHT), 1.0f);
    glBegin(GL_LINES);
        for (int i = 0; i <= columns; ++i)
        {
            glVertex2f(column[i],row[0]);
            glVertex2f(column[i],row[rows]);
        }
        for (int j = 0; j <= columns; ++j)
        {
            glVertex2f(column[0],row[j]);
            glVertex2f(column[columns],row[j]);
        }
		
	glEnd();
    glPopMatrix();
}

void drawBall(int x, int y, float size, float rotx, float roty, float rotz, int texture)
{
    size = size * 0.4; //convert pixel size to coordinate system
    cout << x << " " << y << endl;
    cout << WINDOW_WIDTH << " " << WINDOW_HEIGHT << endl;

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,1.0f*WINDOW_WIDTH/WINDOW_HEIGHT,1,1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    glTranslatef((1.0f*x - WINDOW_WIDTH/2.0f)/WINDOW_WIDTH*2, -(1.0f*y - WINDOW_HEIGHT/2.0f)/WINDOW_HEIGHT*2, -2.0f); //from center
    glRotatef(ballRotX, 1.0f, 0.0f, 0.0f);
    ballRotX += 3;
    glRotatef(ballRotY, 0.0f, 1.0f, 0.0f);
    ballRotY += 2;
    glRotatef(ballRotZ, 0.0f, 0.0f, 1.0f);
    ballRotZ += 1;
    glScalef(2.0f/(WINDOW_WIDTH)*size, 2.0f/(WINDOW_WIDTH)*size, 2.0f/(WINDOW_WIDTH)*size);
    
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_TRIANGLES);
        for (int i = 0; i < numberOfVertices; ++i)
        {
            glColor3f(0.95f, 0.95f, 0.95f);
            //glColor3f(sphereTextureCoordinates[i*2], sphereTextureCoordinates[i*2+1], 0.0f);
            
            glTexCoord2f(sphereTextureCoordinates[i*2], sphereTextureCoordinates[i*2+1]);
            
            glVertex3f(sphereVertices[i*3], sphereVertices[i*3+1], sphereVertices[i*3+2]); 
        }

        /*
        //test triangle
        glColor3f(1,0.1,0.1);
        glVertex3f(0, 50, 0);
        glVertex3f(-33, 80, 0);
        glVertex3f(33, 80, 0);
        */

        /*
        //test cube (quads)
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(-1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,1.0);
        glVertex3f(1.0,1.0,1.0);
        //back
        glColor3f(0.0,1.0,0.0);
        glVertex3f(1.0,1.0,-1.0);
        glVertex3f(1.0,-1.0,-1.0);
        glVertex3f(-1.0,-1.0,-1.0);
        glVertex3f(-1.0,1.0,-1.0);
        //right
        glColor3f(0.0,0.0,1.0);
        glVertex3f(1.0,1.0,1.0);
        glVertex3f(1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,-1.0);
        glVertex3f(1.0,1.0,-1.0);
        //left
        glColor3f(1.0,1.0,0.0);
        glVertex3f(-1.0,1.0,-1.0);
        glVertex3f(-1.0,-1.0,-1.0);
        glVertex3f(-1.0,-1.0,1.0);
        glVertex3f(-1.0,1.0,1.0);
        //top
        glColor3f(0.0,1.0,1.0);
        glVertex3f(-1.0,1.0,-1.0);
        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(1.0,1.0,1.0);
        glVertex3f(1.0,1.0,-1.0);
        //bottom
        glColor3f(1.0,0.0,1.0);
        glVertex3f(-1.0,-1.0,-1.0);
        glVertex3f(-1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,-1.0);
        */
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void reshapeCanvas(int w, int h)
{
    mouseScaleX = WINDOW_WIDTH * 1.0 / w;
    mouseScaleY = WINDOW_HEIGHT * 1.0 / h;

    glViewport(0, 0, w, h);
}

void init()
{
    initColumnsAndRows();
    createButtons();
    loadSet();
    createSphere();
}

void createButtons()
{
    homeScreenButtons.push_back(Button(column[2]+margin, row[3]+margin, column[8]-margin, row[5]-margin, &accentColor, Clickable, stateToEditTeam1));
    homeScreenButtons.push_back(Button(column[10]+margin, row[3]+margin, column[16]-margin, row[5]-margin, &accentColor, Clickable, stateToEditTeam2));
    
    //main score display panels
    homeScreenButtons.push_back(Button(column[2]+margin, row[5]+margin, column[5]-margin, row[13]-margin, &team1Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[5]+margin, row[5]+margin, column[8]-margin, row[13]-margin, &team1Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[10]+margin, row[5]+margin, column[13]-margin, row[13]-margin, &team2Color, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[13]+margin, row[5]+margin, column[16]-margin, row[13]-margin, &team2Color, DisplayOnly, nullptr));

    //add and subtract from score buttons
    homeScreenButtons.push_back(Button(column[2]+margin, row[13]+margin, column[5]-margin, row[15]-margin, &accentColor, Clickable, decrementScore1));
    homeScreenButtons.push_back(Button(column[5]+margin, row[13]+margin, column[8]-margin, row[15]-margin, &accentColor, Clickable, incrementScore1));
    homeScreenButtons.push_back(Button(column[10]+margin, row[13]+margin, column[13]-margin, row[15]-margin, &accentColor, Clickable, decrementScore2));
    homeScreenButtons.push_back(Button(column[13]+margin, row[13]+margin, column[16]-margin, row[15]-margin, &accentColor, Clickable, incrementScore2));

    //set display and set option buttons
    homeScreenButtons.push_back(Button(column[8]+margin, row[5]+margin, column[10]-margin, row[9]-margin, &accentColor, Clickable, stateToEditSetNum));
    homeScreenButtons.push_back(Button(column[8]+margin, row[9]+margin, column[10]-margin, row[11]-margin, &accentColor, Clickable, saveSet));
    homeScreenButtons.push_back(Button(column[8]+margin, row[11]+margin, column[10]-margin, row[13]-margin, &accentColor, Clickable, discardSet));
    
    //timer time panel
    homeScreenButtons.push_back(Button(column[2]+margin, row[16]+margin, column[5]-margin, row[18]-margin, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[5]+margin, row[16]+margin, column[13]-margin, row[18]-margin, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[13]+margin, row[16]+margin, column[16]-margin, row[18]-margin, &accentColor, DisplayOnly, nullptr));

    //time panel
    homeScreenButtons.push_back(Button(column[5]+margin, row[0]+margin, column[13]-margin, row[2]-margin, &accentColor, DisplayOnly, nullptr));
    //settings panel
    homeScreenButtons.push_back(Button(column[17]-margin, row[0]+margin, column[18]-margin, row[2]-margin, &accentColor, DisplayOnly, nullptr));
    //fullscreen panel
    homeScreenButtons.push_back(Button(column[0]+margin, row[0]+margin, column[1]+margin, row[2]-margin, &accentColor, Clickable, fullScreenProgram));

    //set panels
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[3]+margin/2, column[1], row[5]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[5]+margin/2, column[1], row[6]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[6]+margin/2, column[1], row[7]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[7]+margin/2, column[1], row[8]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[8]+margin/2, column[1], row[9]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[9]+margin/2, column[1], row[10]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[10]+margin/2, column[1], row[11]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[11]+margin/2, column[1], row[12]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[12]+margin/2, column[1], row[13]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[13]+margin/2, column[1], row[14]-margin/2, &accentColor, DisplayOnly, nullptr));
    homeScreenButtons.push_back(Button(column[0]+margin/2, row[14]+margin/2, column[1], row[15]-margin/2, &accentColor, DisplayOnly, nullptr));

    //text editing buttons
    editScreenButtons.push_back(Button(70, 240, 350, 330, &accentColor, Clickable, discardInput));
    editScreenButtons.push_back(Button(450, 240, 730, 330, &accentColor, Clickable, saveInput));

}

void createSphere() //create a unit sphere
{
    /*
    one rotational side of a sphere with height detail 2: 

               a
              /\
             /1 \
          b /____\ c
            |   /|
            |2 / |
            | / 3|
          d |/___| e
            \    /
             \4 /
              \/
              f

    triangles counter clockwise/right hand rule
    used from prev:             vv    vv    vv
                        1:abc 2:bcd 3:cde 4:def
    used in next:          ^^    ^^    ^^   
    the same pattern occurs for heights greater than 2

    in the openGL coordinate system:
    positive x points right
    positive y points up
    positive z points out of screen (toward user)
    */
    
    //total vertices = 3*sphereHeightDetail*sphereRotationalDetail*2
    //total triangles = sphereHeightDetail*sphereRotationalDetail*2

    
    glGenTextures(1, &ballTexture);
    glBindTexture(GL_TEXTURE_2D, ballTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int texWidth, texHeight, texnrChannels;
    unsigned char *data = stbi_load("Textures/volleyball_thickened.jpg", &texWidth, &texHeight, &texnrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);


    for (int i = 0; i < sphereRotationalDetail; ++i) //for each column/rotation/side of the sphere
    {
        //make triangle 1, so we can use it to build the others
        //cout << "column start: " << i << endl;
        //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail << endl;
        //a
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail] = cos(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*0/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+1] = sin(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*0/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+2] = cos(M_PI*0/(sphereHeightDetail+1)); //z
        //cout << setprecision(2) << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+1] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+2] << " A " << endl;

        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail] = (float)i/sphereRotationalDetail;
        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+1] = (float)0/(sphereHeightDetail+1);

        //b
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+3] = cos(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+4] = sin(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+5] = cos(M_PI*1/(sphereHeightDetail+1)); //z

        //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail+3 << endl;
        //cout << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+3] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+4] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+5] << " B " << endl;

        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+2] = (float)i/sphereRotationalDetail;
        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+3] = (float)0/(sphereHeightDetail+1);

        //c
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+6] = cos(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+7] = sin(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+8] = cos(M_PI*1/(sphereHeightDetail+1)); //z

        //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail+6 << endl;
        //cout << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+6] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+7] << " "
        //    << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+8] << " C " << endl << endl;

        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+4] = (float)i/sphereRotationalDetail;
        //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+5] = (float)0/(sphereHeightDetail+1);

        for (int j = 1; j < sphereHeightDetail*2; ++j) //for each triangle within the column, besides the first
        {
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+3]; //x from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+1] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+4]; //y from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+2] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+5]; //z from previous triangle

            //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail+j*9 << " taking from index: " << i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+3 << endl;
            //cout << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+1] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+2] << " a " << endl;

            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6] = (float)i/sphereRotationalDetail;
            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6+1] = (float)j/(sphereHeightDetail+1);

            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+3] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+6]; //x from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+4] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+7]; //y from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+5] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+8]; //z from previous triangle
            
            //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail+j*9+3  << " taking from index: " << i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+6 << endl;
            //cout << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+3] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+4] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+5] << " b " << endl;
            
            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6+2] = (float)i/sphereRotationalDetail;
            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6+3] = (float)j/(sphereHeightDetail+1);

            if (j % 2 == 0) //if the next triangle vertice to make is to finish a square, make sure it is generated with the right hand rule
            {
                //cout << "odd num" << endl;
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+6] = cos(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*(int((j+2)/2))/(sphereHeightDetail+1)); //new x
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+7] = sin(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*(int((j+2)/2))/(sphereHeightDetail+1)); //new y
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+8] = cos(M_PI*(int((j+2)/2))/(sphereHeightDetail+1)); //new z
            }
            else 
            {
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+6] = cos(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*(int((j+1)/2)+1)/(sphereHeightDetail+1)); //new x
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+7] = sin(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*(int((j+1)/2)+1)/(sphereHeightDetail+1)); //new y
                sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+8] = cos(M_PI*(int((j+1)/2)+1)/(sphereHeightDetail+1)); //new z
            }

            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6+4] = (float)i/sphereRotationalDetail;
            //sphereTextureCoordinates[i*numberOfCoordinates/sphereRotationalDetail+j*6+5] = (float)j/(sphereHeightDetail+1);

            //cout << "index: " << i*numberOfCoordinates/sphereRotationalDetail+j*9+6 << endl;
            //cout << sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+6] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+7] << " "
            //<< sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+8] << " c " << endl << endl;
        }
        for (int j = 0; j < sphereHeightDetail*2; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                int m = j + k;
                int col = (m == 0 || m % 2 == 1) ? i : i + 1;
                int lat = (m == 0) ? 0 : (m + 1) / 2;

                int idx = (i * sphereHeightDetail * 2 * 3 + j * 3 + k) * 2;
                sphereTextureCoordinates[idx]     = (float)col / sphereRotationalDetail; // u
                sphereTextureCoordinates[idx + 1] = (float)lat / (sphereHeightDetail + 1); // v
            }
        }
    }
}
void initColumnsAndRows()
{
    for (int i = 0; i <= columns; ++i)
    {
        column[i] = i*WINDOW_WIDTH/columns;
    }

    for (int i = 0; i <= rows; ++i)
    {
        row[i] = i*WINDOW_HEIGHT/rows;
    }
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
            toInt(inputString, setToLoad);
            loadSet();
            game[setNumber - 1].setSetNumber(setNumber);
            break;
        default:
            break;
    }
    discardInput(); //clean up input messes and set program state back to home
    stateToHomeScreen();
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
void incrementScore1()
{
    game[setNumber - 1].incrementScore1();
}
void incrementScore2()
{
    game[setNumber - 1].incrementScore2();
}
void decrementScore1()
{
    game[setNumber - 1].decrementScore1();
}
void decrementScore2()
{
    game[setNumber - 1].decrementScore2();
}
void resetScore()
{
    game[setNumber - 1].resetScore();
}
void saveSet()
{
    //todo
    //put score1, score2, and timer time into an object in a list at index set - 1
    //maybe also store team colors and names

}

void loadSet()
{
    //if the set is already loaded, abort
    if (setToLoad == setNumber && game.size() >= setNumber)
        return;
    
    if (setToLoad <= 0)
        setToLoad = 1;

    //if the set to load does not exist, create a new set
    if (setToLoad > game.size() + 1) //if the set to load is higher than the game vector size, create a new set at the end of the list
    {
        game.push_back(Set());
        setToLoad = game.size();
    }
    else if (setToLoad == game.size() + 1 && game.size() > 0) //if the set to load is just after the end of the list, create a new set based on the current set at the end of the list
    {
        game.push_back(Set(game[setNumber - 1], setToLoad));
    }
    else if (setToLoad == game.size() + 1) //if there are no sets
    {
        game.push_back(Set());
    }

    //the set to load has been found or created, now load the data from it
    setNumber = game[setToLoad - 1].getSetNumber();
    if (setNumber != setToLoad) //if the loading number and object number are not the same, use the loading number
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

void fullScreenProgram()
{
    fullScreen = !fullScreen;
    if (fullScreen)
    {
        glutFullScreen();
    }
    else
    {
        glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT); 
        glutPositionWindow(300, 100);
    }
}






