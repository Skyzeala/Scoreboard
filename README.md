# Scoreboard program

A program designed to keep track of scores for games like volleyball, basketball, and ping pong.

You can find concept art for the interface design in concept_art.png which shows a rough sketch of the planned features.

## Technical details
* C++
* OpenGL
* freeGLUT
* ctime

## Build instructions for Windows 10 using VSCode (unfinished)

You will need MinGW and freeglut and then you must tell VSCode where to look for the libraries using a .vscode folder and subsequent files

## Current and future features
* Current time of day updated every second
* Display scores between 00 and 99 for two teams
* Draw boxes using pixel coordinates
* Print text on the screen that is realistically sized
* Buttons to add or subtract a point from each team's score
* Team names can be edited in a GUI
* Set number can be edited in a GUI
* Window can be resized to users needs
* Store and display past saved set scores and data
* Display and manage a stopwatch for the game's time
* A settings page to allow a user to change team colors, text color, and background color



## Sources and attributions:
Thank you to Lighthouse3d for the introductive tutorials to help me check my framework setup.
* I used some of the code from this tutorial:
http://www.lighthouse3d.com/tutorials/glut-tutorial/preparing-the-window-for-a-reshape/

I also used code from these stackoverflow questions and answers:
* https://stackoverflow.com/questions/27276075/glut-mouse-button-down
* https://stackoverflow.com/questions/24872986/error-a-pointer-to-a-bound-function-may-only-be-used-to-call-the-function



