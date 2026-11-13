#ifndef _SBBALLSPHERE_
#define _SBBALLSPHERE_

#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif



class Sphere {

    public:
        Sphere();
        Sphere(const Sphere &sphere);
        ~Sphere();
        void drawSphere(int xpos, int ypos, float size, int winWidth, int winHeight);
        void updateSphere();

    private:
        static const int sphereHeightDetail = 10; //number of vertices between top and bottom of sphere
        static const int sphereRotationalDetail = 30; //number of vertices around the sides
        static const int numberOfTriangles = sphereHeightDetail*sphereRotationalDetail*2;
        static const int numberOfVertices = 3*numberOfTriangles;
        static const int numberOfCoordinates = 3*numberOfVertices;
        GLfloat sphereVertices[numberOfCoordinates]; //x and y and z coordinates for each triangle point
        GLfloat sphereTextureCoordinates[numberOfCoordinates];
        unsigned int ballTexture;
        int ballRotX;
        int ballRotY;
        int ballRotZ;
        //int ballRotVelocityX = 0;
        //int ballRotVelocityY = 0;
        //int ballRotVelocityZ = 0;

};






#endif