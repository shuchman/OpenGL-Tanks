#include "map.h"
#include "player.h"
#include <GL/glut.h>
#include <iostream>
#include <jpeglib.h>

#define PI 3.1416
#define UP_dir 180
#define DOWN_dir  0
#define RIGHT_dir  90
#define LEFT_dir 270

using namespace std;
void display();
void keyboard(unsigned char c,int x,int y);
void idle();
void maze_to_binary();
void position_observer(float alpha, float beta, int rad);
void draw_cube(int size, int i, int j);
void ReadJPEG(char *filename,unsigned char **image,int *width, int *height);
void LoadTexture(char *filename,int dim);


Map cat;
Player me, enemy;
int keyflag=0;
int COLUMNS;
int ROWS;
int WIDTH;
int HEIGHT;
long last_t = 0;
bool maze[500][500];
int delay = 300;
int angle_alpha = 90;
int angle_beta = 1;
int maze_height;
GLuint textures[3];

int main(int argc, char *argv[])
{
    cout << "insert columns greater than 5 - ";
    cin >> COLUMNS;
    cout << "insert rows greater than 5 - ";
    cin >> ROWS;
    if ( ( COLUMNS < 5 ) | ( ROWS < 5 ) ) {
        cout << "invalid parameters";
        return 0;
    }
    if ( COLUMNS % 2 == 0 )
        COLUMNS++;
    if ( ROWS % 2 == 0 )
        ROWS++;
    cat.maze_size[0] = COLUMNS;
    cat.maze_size[1] = ROWS;
    me.init_me();
    enemy.init_enemy(ROWS, COLUMNS);
    WIDTH = 30 * COLUMNS;
    HEIGHT = 30 * ROWS;
    me.set_position(15, 15);
    enemy.set_position((ROWS-2)*15, (COLUMNS-2)*15);
    cat.map_constructor();
    cat.printMaze();
    maze_to_binary();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Maze");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);


    glGenTextures(3, textures);
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    LoadTexture("floor2.jpg",64);
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    LoadTexture("top.jpeg",64);
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    LoadTexture("tanktex.jpeg",64);

    glutMainLoop();
    return 0;
}

void display() {
    int i, j, size;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    position_observer(angle_alpha, angle_beta, 450);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH*0.6,WIDTH*0.6,-HEIGHT*0.6,HEIGHT*0.6,10,2000);
    glMatrixMode(GL_MODELVIEW);
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);

    for (i = 0; i < COLUMNS; i++) {
        for (j = 0; j < ROWS; j++) {
            if (!maze[j][i]) {
                size = 15;
                maze_height = 5;
                draw_cube(size, i, j);


            }
        }
    }

    {   // maze floor
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3i(0, 0, 0);
        glTexCoord2f(4.0, 0.0);
        glVertex3i(0, 0, ROWS * size);
        glTexCoord2f(4.0, 4.0);
        glVertex3i(COLUMNS * size, 0, ROWS * size);
        glTexCoord2f(0.0, 4.0);
        glVertex3i(COLUMNS * size, 0, 0);
        glEnd();
    }

    glPushMatrix();
    glTranslatef(me.y+8,0,me.x+8);
    glRotatef(me.rotata, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    me.draw(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(enemy.y+8,0,enemy.x+8);
    glRotatef(enemy.rotata, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    enemy.draw(0,0,0);
    glPopMatrix();

    {   // text printer
        glColor3f(1, 1, 0);
        glRasterPos3f(WIDTH/2, 15, HEIGHT/2);
        int timer = 60 - glutGet(GLUT_ELAPSED_TIME)/1000;
        char str[2];
        sprintf(str,"%d",timer);
        char *to_print = "Time Remaining: ";
        while (*to_print) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *to_print++);
        }
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *(str+1));
        if(timer == 0)
            exit(0);
    }
    glutSwapBuffers();
}

void draw_cube(int size,int i,int j){
    glColor3f(1.0, 0.0, 0.0);   //bottom red
    glBegin(GL_QUADS);
    glVertex3i(i*size, 0, j*size);
    glVertex3i((i+1)*size, 0, j*size);
    glVertex3i((i+1)*size, 0, (j+1)*size);
    glVertex3i(i*size, 0, (j+1)*size);
    glEnd();


//    glColor3f(1.0, 1.0, 1.0);   //top white
//    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0,0.0); glVertex3i(i*size, maze_height, j*size);
    glTexCoord2f(1.0,0.0);  glVertex3i(i*size, maze_height, (j+1)*size);
    glTexCoord2f(1.0,1.0);  glVertex3i((i+1)*size, maze_height, (j+1)*size);
    glTexCoord2f(0.0,1.0); glVertex3i((i+1)*size, maze_height, j*size);
    glEnd();


    glColor3f(0.0, 0.0, 1.0);   //side 1, blue
    glBegin(GL_QUADS);
    glVertex3i(i*size, maze_height, (j+1)*size);
    glVertex3i(i*size, 0, (j+1)*size);
    glVertex3i((i+1)*size, 0, (j+1)*size);
    glVertex3i((i+1)*size, maze_height, (j+1)*size);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);   //it's counter-part, green
    glBegin(GL_QUADS);
    glVertex3i(i*size, maze_height, j*size);
    glVertex3i((i+1)*size, maze_height, j*size);
    glVertex3i((i+1)*size, 0, j*size);
    glVertex3i(i*size, 0, j*size);
    glEnd();

    glColor3f(0.8,0.2,0.2); //side 3 - weirmad reddish color
    glBegin(GL_QUADS);
    glVertex3i((i+1)*size, maze_height, (j+1)*size);
    glVertex3i((i+1)*size, 0, (j+1)*size);
    glVertex3i((i+1)*size, 0, j*size);
    glVertex3i((i+1)*size, maze_height, j*size);
    glEnd();

    glColor3f(0.2,0.8,0.2); //side 4 - weird reddish color
    glBegin(GL_QUADS);
    glVertex3i(i*size, maze_height, (j+1)*size);
    glVertex3i(i*size, maze_height, j*size);
    glVertex3i(i*size, 0, j*size);
    glVertex3i(i*size, 0, (j+1)*size);
    glEnd();
}   //draws a cube

void maze_to_binary() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if ( i == ROWS-1 || i==0 || j==COLUMNS-1 || j==0)
                maze[i][j]=false;
            else
                maze[i][j]=!cat.maze[i][j][0];
        }
    }

}   //converts generated maze to binary

void keyboard(unsigned char c,int x,int y)
{
    if(me.state == 0) {
        {   //movement keys
            if ((c == 'W') | (c == 'w')) {
                int dir  = me.rotata%360;
                if (dir < 0 ){
                    dir = dir + 360;
                }

                switch (dir) {
                    case UP_dir :
                        if (maze[me.x_index - 1][me.y_index] &((me.x_index - 1 != enemy.x_index) | (me.y_index != enemy.y_index))) {
                            --me.x_index;
                            me.init_movement(me.x - 15, me.y, 300);
                        }
                        break;

                    case DOWN_dir:
                        if (maze[me.x_index + 1][me.y_index] & ((me.x_index + 1 != enemy.x_index) | (me.y_index != enemy.y_index))){
                            ++me.x_index;
                            me.init_movement(me.x+15,me.y,300);
                        }
                        break;

                    case RIGHT_dir:
                        if (maze[me.x_index][me.y_index + 1]& ((me.x_index != enemy.x_index) | (me.y_index + 1 != enemy.y_index))){
                            ++me.y_index;
                            me.init_movement(me.x,me.y+15,300);

                        }
                        break;

                    case LEFT_dir:
                        if (maze[me.x_index][me.y_index - 1] & ((me.x_index != enemy.x_index) | (me.y_index - 1 != enemy.y_index))){
                            --me.y_index;
                            me.init_movement(me.x,me.y-15,300);
                        }
                        break;


                }
            }

            {
                // case UP: (maze[me.x_index - 1][me.y_index] &((me.x_index - 1 != enemy.x_index) | (me.y_index != enemy.y_index)))
                // case LEFT: (maze[me.x_index][me.y_index - 1] & ((me.x_index != enemy.x_index) | (me.y_index - 1 != enemy.y_index)))
                // case RIGHT: (maze[me.x_index][me.y_index + 1]) & ((me.x_index != enemy.x_index) | (me.y_index + 1 != enemy.y_index)))
                // case DOWN: (maze[me.x_index + 1][me.y_index] & ((me.x_index + 1 != enemy.x_index) | (me.y_index != enemy.y_index))
            }

            if ((c == 'A') | (c == 'a')) {
                me.init_rotation(me.rotata+90, 300);
            }

            if ((c == 'D') | (c == 'd')) {
                me.init_rotation(me.rotata-90, 300);
            }
        }

//        cout<<"ME after movement: "<<"("<<me.x_index<<","<<me.y_index<<")\n";
//        cout<<"ENEMY: "<<"("<<enemy.x_index<<","<<enemy.y_index<<")\n\n";

        {   //observation keys
            if (c == 'i' && angle_beta <= (90 - 4))
                angle_beta = (angle_beta + 3);
            else if (c == 'k' && angle_beta >= (-90 + 4))
                angle_beta = angle_beta - 3;
            else if (c == 'j')
                angle_alpha = (angle_alpha + 3) % 360;
            else if (c == 'l')
                angle_alpha = (angle_alpha - 3 + 360) % 360;
            glutPostRedisplay();
        }
    }
}   //handles keyboard input

void idle() {
    long t;

    t = glutGet(GLUT_ELAPSED_TIME);

    if (last_t == 0)
        last_t = t;
    else {
        me.integrate(t - last_t);
        enemy.integrate(t - last_t);
        last_t = t;
    }
    if (enemy.state == 0) {
        int num = rand() % 4;
        if (num == 0) {
            if (maze[enemy.x_index + 1][enemy.y_index] & ((enemy.x_index + 1 != me.x_index) | (enemy.y_index != me.y_index))) {
                enemy.init_movement(enemy.x+15,enemy.y,1000);
                enemy.x_index++;
            }
        }
        else if (num == 1) {
            if (maze[enemy.x_index - 1][enemy.y_index] & ((enemy.x_index - 1 != me.x_index) | (enemy.y_index != me.y_index))) {
                enemy.init_movement(enemy.x-15,enemy.y,1000);
                enemy.x_index--;
            }
        }
        else
            enemy.init_rotation(enemy.rotata+90, 1000);
    }

    glutPostRedisplay();
}   //stuffs

void position_observer(float alpha,float beta,int radi)
{
    float x,y,z;
    float upx,upy,upz;
    float modul;

    x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
    y = (float)radi*sin(beta*2*PI/360.0);
    z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

    if (beta>0)
    {
        upx=-x;
        upz=-z;
        upy=(x*x+z*z)/y;
    }
    else if(beta==0)
    {
        upx=0;
        upy=1;
        upz=0;
    }
    else
    {
        upx=x;
        upz=z;
        upy=-(x*x+z*z)/y;
    }


    modul=sqrt(upx*upx+upy*upy+upz*upz);

    upx=upx/modul;
    upy=upy/modul;
    upz=upz/modul;

//    gluLookAt(x,y,z,    0.0, 0.0, 0.0,     upx,upy,upz);    //normal view
    gluLookAt(x+WIDTH/4,y,z+HEIGHT/4,    0.0+WIDTH/4, 0.0, 0.0+HEIGHT/4,     upx,upy,upz);  //better view
}   //positions the user observation point

void ReadJPEG(char *filename,unsigned char **image,int *width, int *height)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;
    unsigned char **buffer;
    int i,j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n",filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height  = cinfo.output_height;


    *image=(unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

    buffer=(unsigned char **)malloc(1*sizeof(unsigned char **));
    buffer[0]=(unsigned char *)malloc(cinfo.output_width*cinfo.output_components);


    i=0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for(j=0;j<cinfo.output_width*cinfo.output_components;j++)
        {
            (*image)[i]=buffer[0][j];
            i++;
        }

    }

    free(buffer);
    jpeg_finish_decompress(&cinfo);
}

void LoadTexture(char *filename,int dim)
{
    unsigned char *buffer;
    unsigned char *buffer2;
    int width,height;
    long i,j;
    long k,h;

    ReadJPEG(filename,&buffer,&width,&height);

    buffer2=(unsigned char*)malloc(dim*dim*3);

    //-- The texture pattern is subsampled so that its dimensions become dim x dim --
    for(i=0;i<dim;i++)
        for(j=0;j<dim;j++)
        {
            k=i*height/dim;
            h=j*width/dim;

            buffer2[3*(i*dim+j)]=buffer[3*(k*width +h)];
            buffer2[3*(i*dim+j)+1]=buffer[3*(k*width +h)+1];
            buffer2[3*(i*dim+j)+2]=buffer[3*(k*width +h)+2];

        }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim,dim,0,GL_RGB,GL_UNSIGNED_BYTE,buffer2);

    free(buffer);
    free(buffer2);
}