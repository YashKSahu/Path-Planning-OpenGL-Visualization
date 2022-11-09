#include <GL/glut.h>
#include <bits/stdc++.h>

int onMouse;
float r,g,b;
// int HEIGHT=720,WIDTH=720;
int DIM=720;

void init(){
    glClearColor(1,1,1,0);  //black background
    // glLoadIdentity();
    // gluOrtho2D(0, 640, 0, 480);
    glOrtho(0, DIM, 0, DIM,-1,1);
}

void pointPlot(int x, int y, int center_x, int center_y){
    glPointSize(2);
    glBegin(GL_POINTS);
        glVertex2i(x+center_x,y+center_y);
    glEnd();
}

void drawLine(int x1,int y1, int x2, int y2){
    glLineWidth(1.0);
    glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
    glEnd();
}

void drawGrids(int size){
    glColor3f(0.0, 0.0, 0);
    for(int i=0; i<=DIM-size; i=i+size){
        drawLine(i,0,i,DIM);
        drawLine(0,i,DIM,i);
        
        // Maze Boundaries
        glRecti(i, 0, i+size, size);
        glRecti(i, DIM-size, i+size, size+DIM-size);
        glRecti(0, i, size, i+size);
        glRecti(DIM-size, i, size+DIM-size, i+size);
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 1.0);

    //    glRecti(25, 25, 400, 400);

    drawGrids(20);

    glFlush();
}


// void display(){
//     glClear(GL_COLOR_BUFFER_BIT);
//     glColor3f(r/10,g/10,b/10);  // random colors
    
//     // Axis
//     glBegin(GL_LINES);
//         glVertex2d(HEIGHT/2,WIDTH);
//         glVertex2d(HEIGHT/2, 0);

//         glVertex2d(0,WIDTH/2);
//         glVertex2d(HEIGHT,WIDTH/2);
//     glEnd();

//     // drawLogic(center_x, center_y, params);

//     glFlush();
// }

void mouseClicks(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        onMouse = 1;
    }
    if(onMouse == 1){
        
        r=rand()%9;
        g=rand()%9;
        b=rand()%9;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv){
    srand(time(0));
    r=rand()%9, g=rand()%9, b=rand()%9;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(DIM, DIM);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Path Planning OpenGL Visualization");
    glutDisplayFunc(display);
    //glutMouseFunc(mouseClicks);
    init();
    glutMainLoop();
}