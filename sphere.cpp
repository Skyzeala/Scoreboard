#include "sphere.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sphere::Sphere() //create a unit sphere
{
    ballRotX = 0;
    ballRotY = 0;
    ballRotZ = 0;

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
        //std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    for (int i = 0; i < sphereRotationalDetail; ++i) //for each column/rotation/side of the sphere
    {
        //make triangle 1, so we can use it to build the others

        //a
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail] = cos(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*0/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+1] = sin(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*0/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+2] = cos(M_PI*0/(sphereHeightDetail+1)); //z

        //b
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+3] = cos(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+4] = sin(M_PI*2*i/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+5] = cos(M_PI*1/(sphereHeightDetail+1)); //z

        //c
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+6] = cos(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //x
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+7] = sin(M_PI*2*(i+1)/sphereRotationalDetail)*sin(M_PI*1/(sphereHeightDetail+1)); //y
        sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+8] = cos(M_PI*1/(sphereHeightDetail+1)); //z

        for (int j = 1; j < sphereHeightDetail*2; ++j) //for each triangle within the column, besides the first
        {
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+3]; //x from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+1] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+4]; //y from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+2] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+5]; //z from previous triangle

            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+3] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+6]; //x from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+4] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+7]; //y from previous triangle
            sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+j*9+5] = sphereVertices[i*numberOfCoordinates/sphereRotationalDetail+(j-1)*9+8]; //z from previous triangle
            
            if (j % 2 == 0) //if the next triangle vertice to make is to finish a square, make sure it is generated with the right hand rule
            {
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
        }

        for (int j = 0; j < sphereHeightDetail*2; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                int m = j+k;
                int col = ((m == 0 || m % 2 == 1) ? i : i+1);
                int lat = ((m == 0) ? 0 : (m+1)/2);

                int index = (i*sphereHeightDetail*2*3 + j*3 + k)*2;
                sphereTextureCoordinates[index] = (float)col/sphereRotationalDetail;   
                sphereTextureCoordinates[index+1] = (float)lat/(sphereHeightDetail+1); 
            }
        }
    }
}


Sphere::Sphere(const Sphere &sphere) : 
    ballTexture(ballTexture), ballRotX(ballRotX), ballRotY(ballRotY), ballRotZ(ballRotZ)
{
    for (int i = 0; i < this->numberOfCoordinates; i++) {
        this->sphereVertices[i] = sphereVertices[i];
        this->sphereTextureCoordinates[i] = sphereTextureCoordinates[i];
    }
}



Sphere::~Sphere() 
{
    //glDeleteTextures(1, &ballTexture);
}


void Sphere::drawSphere(int xpos, int ypos, float size, int winWidth, int winHeight)
{
    size = size * 0.4; //convert pixel size to coordinate system

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,1.0f*winWidth/winHeight,1,1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    glTranslatef((1.0f*xpos - winWidth/2.0f)/winWidth*2, -(1.0f*ypos - winHeight/2.0f)/winHeight*2, -2.0f); //from center
    glRotatef(ballRotX, 1.0f, 0.0f, 0.0f);
    ballRotX += 3;
    glRotatef(ballRotY, 0.0f, 1.0f, 0.0f);
    ballRotY += 2;
    glRotatef(ballRotZ, 0.0f, 0.0f, 1.0f);
    ballRotZ += 1;
    glScalef(2.0f/(winWidth)*size, 2.0f/(winWidth)*size, 2.0f/(winWidth)*size);
    
    glBindTexture(GL_TEXTURE_2D, ballTexture);

    glBegin(GL_TRIANGLES);
        for (int i = 0; i < numberOfVertices; ++i)
        {
            glColor3f(0.95f, 0.95f, 0.95f);
            //glColor3f(sphereTextureCoordinates[i*2], sphereTextureCoordinates[i*2+1], 0.0f);
            
            glTexCoord2f(sphereTextureCoordinates[i*2], sphereTextureCoordinates[i*2+1]);
            
            glVertex3f(sphereVertices[i*3], sphereVertices[i*3+1], sphereVertices[i*3+2]); 
        }
    glEnd();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Sphere::updateSphere() {

}
