#include "player.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
using namespace std;
Player::Player() {
    state = 0;
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.14;
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y,z); // center of circle
    for(i = 0; i <= triangleAmount;i++) {
        glVertex3f(
                x + (radius * cos(i *  twicePi / triangleAmount)),
                z + (radius * sin(i * twicePi / triangleAmount)),y

        );
    }
    glEnd();
}

void Player::init_me(){
    x_index = 1;
    y_index = 1;
}

void Player::init_enemy(int x, int y){
    x_index=x-2;
    y_index=y-2;
}

void Player::set_position(int x, int y) {
    this->x = x;
    this->y = y;
}

void Player::init_movement(float destination_x,float destination_y,float duration)
{
    vx = (destination_x - x)/duration;
    vy = (destination_y - y)/duration;

    this->desx = destination_x;
    this->desy = destination_y;

    state=1;
    time_remaining=duration;
}

void Player::init_rotation(float degree, float duration)
{
    vdeg = (degree - rotata)/duration;
    this->desdeg = degree;
    state=2;
    time_remaining=duration;
}


void Player::integrate(long t)
{
    if(state==2 && t<time_remaining)
    {
        rotata = rotata + vdeg*t;
        time_remaining-=t;
    }
    else if(state==1 && t<time_remaining)
    {
        x = x + vx*t;
        y = y + vy*t;
        time_remaining-=t;
    }
    else if(state==1 && t>=time_remaining)
    {
        x = desx;
        y = desy;
        state=0;
    }
    else if(state==2 && t>=time_remaining)
    {
        rotata = desdeg;
        state=0;
    }
}

void Player::draw(int size, int i, int j) {

    glPushMatrix();
    glScalef(0.6,0.6,0.6);
    drawTank(-9,0,-9);
    glPopMatrix();
}

//// tank is drawen asumeing x & z plan is ground, y axis directing up& dwon. the point (x,y,z) is the bottom left corner of the back,
/// tank is facing to the positive direction of z axis.
void Player::drawTank(int x ,int y, int z){
    int h = 8;
    int w = 18;
    int l = 18;
    float weels = (l*1.2)/10;
    float canon = l*1.2;
    float canon_radiu = l/10;
    float tower_radius= w/2;
    float tower_high = w/2;

//    cout<< x<< "  " << z;

    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);

//  glColor3f(1.0, 0.0, 0.0);


    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
/// 1 back of tank:
//bot left ,top left. top right. bot right
    glVertex3f(x, y, z);// bottom left 2
    glVertex3f(x, y+h, z);// top left 1
    glVertex3f(x+w, y+h, z);// top right 3
    glVertex3f(x+w, y, z);// bottom right 4

    glEnd();

//  glColor3f(1.0, 0.0, 0.0);

/// 2 right side of tank:
    // glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glColor3f(0.8, 0.6, 0.2);
    glVertex3f(x+w, y, z+l);// bottom left 8
    glVertex3f(x+w, y, z);// bottom right 4
    glVertex3f(x+w, y+h, z);// top right 3
    glVertex3f(x+w, y+h, z+l);// top left 7
    glEnd();

/// 3 front of tank

    glBegin(GL_QUADS);
    glColor3f(0.5, 0.6, 0.2);
    glVertex3f(x, y, z+l);// bottom left 6
    glVertex3f(x+w, y, z+l);// bottom right 8
    glVertex3f(x+w, y+h, z+l);// top right 7
    glVertex3f(x, y+h, z+l);// top left 5
    glEnd();

/// 4 left size of tank
    glBegin(GL_QUADS);
    glColor3f(0.8, 0.6, 0.2);
    glVertex3f(x, y, z+l);// bottom left 6
    glVertex3f(x, y+h, z+l);// top left 5
    glVertex3f(x, y+h, z);// top left 1
    glVertex3f(x, y, z);// bottom left 2
    glEnd();
//  glColor3f(0.0, 0.0, 0.0);

/// 5 up of tank

    glBegin(GL_QUADS);
    glColor3f(0.5, 0.6, 0.2);
    glTexCoord2f(0.0,0.0); glVertex3f(x, y+h, z);// top left 1
    glTexCoord2f(1.0,0.0); glVertex3f(x, y+h, z+l);// top left 5
    glTexCoord2f(1.0,1.0); glVertex3f(x+w, y+h, z+l);// top right 7
    glTexCoord2f(0.0,1.0); glVertex3f(x+w, y+h, z);// top right 3
    glEnd();
/// 6 down of tank
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(x, y, z+l);// bottom left 6
    glVertex3f(x, y, z);// bottom left 2
    glVertex3f(x+w, y, z);// top right 4
    glVertex3f(x+w, y, z+l);// bottom right 8
    glEnd();

    GLUquadricObj* myReusableQuadric = gluNewQuadric();
    glColor3f(5.0,5.0,0.0);
    glTranslatef(x-1 ,y,z-1);
    glRotatef(90,0,1,0);
    // glTranslatef(x+ 0,y+ 0,z-2 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (-l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (-l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef((-l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (-l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    //////////////////////////////////////////////////////
    glTranslatef( 0, 0, w );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    glTranslatef( (l*1.2)/5, 0, 0 );
    gluCylinder( myReusableQuadric, weels, weels, weels, 30, 30 );
    /////////////////////////////////////////////////////
    glRotatef(90,0,1,0);
    // glLoadIdentity();
    glTranslatef( w/3+2, h*1.2, -l*1.8);
    gluCylinder( myReusableQuadric, canon_radiu, canon_radiu, canon, 30, 30 );

    /////////////////////////////////////////////////////
    // glRotated()
    glColor3f(0.7,0.3,0.4);
    glRotated(90,1,0,0);
    glTranslated(0,27,-4);
    gluCylinder( myReusableQuadric, tower_radius,tower_radius, tower_high, 30, 30 );


    glColor3f(0.0,0.9,0.0);
    gluDisk(	myReusableQuadric,
                0,
                tower_radius,
                300,
                100);

    glColor3f(0.9,0.0,0.2);
    GLfloat cx,cy,cz;
    cx =10;
    cy =-10;
    cz=20;
    //radi = 200;

    // drawFilledCircle(cx,cy,cz,tower_radius);
}