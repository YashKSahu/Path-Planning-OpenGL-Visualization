#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int onMouse;
float r,g,b;
// int HEIGHT=720,WIDTH=720;
int DIM=720;
int GRID_SIZE=20;

vector<int> grids;

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

void drawObstacles(int x, int y){
    auto it_x = lower_bound(grids.begin(), grids.end(),x);
    x = *it_x-GRID_SIZE;
    auto it_y = lower_bound(grids.begin(), grids.end(),y);
    y = *it_y-GRID_SIZE;
    glRecti(x, y, x+GRID_SIZE, y+GRID_SIZE);
    glFlush();
}

void drawGrids(int GRID_SIZE){
    glColor3f(0.0, 0.0, 0);
    for(int i=0; i<=DIM-GRID_SIZE; i=i+GRID_SIZE){
        drawLine(i,0,i,DIM);
        drawLine(0,i,DIM,i);
        
        // Maze Boundaries
        glRecti(i, 0, i+GRID_SIZE, GRID_SIZE);
        glRecti(i, DIM-GRID_SIZE, i+GRID_SIZE, GRID_SIZE+DIM-GRID_SIZE);
        glRecti(0, i, GRID_SIZE, i+GRID_SIZE);
        glRecti(DIM-GRID_SIZE, i, GRID_SIZE+DIM-GRID_SIZE, i+GRID_SIZE);
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 1.0);

    //pre-processing
    for(int i=0; i<DIM; i=i+GRID_SIZE) grids.push_back(i);
    
    drawGrids(GRID_SIZE);
    // drawRandomObstacles(10);

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

// void mouseClicks(int button, int state, int x, int y){
//     if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
//         onMouse = 1;
//     }
//     if(onMouse == 1){
        
//         r=rand()%9;
//         g=rand()%9;
//         b=rand()%9;
//     }
//     glutPostRedisplay();
// }

void mouseClick(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int xi = x;
        int yi = DIM-y;
        // cout<<x<<" "<<y<<"\n";
        drawObstacles(xi, yi);
    }
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
    glutMouseFunc(mouseClick);
    init();
    glutMainLoop();
}