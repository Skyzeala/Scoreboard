# Scoreboard program

A program designed to keep track of scores for games like volleyball, basketball, and ping pong.

You can find concept art for the interface design in concept_art.png which shows all of the planned features.

## Technical details
* C++
* OpenGL
* freeGLUT
* ctime

## Build instructions for Windows 10 using VSCode (unfinished)

You will need MinGW and freeglut and then you must tell VSCode where to look using a .vscode folder and subsequent files

## Current and future features
* Current time of day updated every second
* Display scores between 00 and 99 for two teams
* Draw boxes using pixel coordinates
* Print text on the screen that is realistically sized
* Buttons to add or subtract a point from each team's score
* Allow the user to edit team names in a gui, the team names will be displayed on screen
* Display the game's set number and allow a user to change it
* Store and display past saved set scores
* Display and manage a stopwatch for the game's time
* A settings page to allow a user to change team colors, text color, and background color
* Accurate window resize behavior


## Sources and attributions:
Thank you to Lighthouse3d for the introductive tutorials to help me check my framework setup.
I used some of the code from this tutorial:
http://www.lighthouse3d.com/tutorials/glut-tutorial/preparing-the-window-for-a-reshape/

I also used code from these stackoverflow questions and answers:
https://stackoverflow.com/questions/27276075/glut-mouse-button-down



