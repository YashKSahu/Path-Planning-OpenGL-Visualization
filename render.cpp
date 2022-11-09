#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int onMouse;
float r,g,b;
// int HEIGHT=720,WIDTH=720;
int DIM=720;
int GRID_SIZE=30;

bool START=false;
bool GOAL=false;

vector<int> grids;

void init(){
    glClearColor(1,1,1,0);  //black background
    // glLoadIdentity();
    // gluOrtho2D(0, 640, 0, 480);
    glOrtho(0, DIM, 0, DIM,-1,1);
}

void setColor(string color){
    if(color=="black")              // obstacles and boundaries
        glColor3f(0.0, 0.0, 0.0);
    if(color=="white")              // free, movable space
        glColor3f(1.0, 1.0, 1.0);
    if(color=="red")                // goal
        glColor3f(1.0, 0.0, 0.0);
    if(color=="green")              // path
        glColor3f(0.0, 1.0, 0.0);
    if(color=="blue")               // start
        glColor3f(0.0, 0.0, 1.0);
    return;
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

void drawBlock(int x, int y, string color){

    //check color
    float pixel_color[3];
    glReadPixels(x,y,1.0,1.0,GL_RGB,GL_FLOAT,pixel_color);
    // cout<<pixel_color[0]<<" "<<pixel_color[1]<<" "<<pixel_color[2]<<"\n";
    if(round(pixel_color[0])!=1 or round(pixel_color[1])!=1 or round(pixel_color[2])!=1) return;

    if(START==false){
        color = "blue";     //******start*****
        START=true;
    }
    else if(GOAL==false){
        color = "red";      //******goal*****
        GOAL=true;
    }

    auto it_x = lower_bound(grids.begin(), grids.end(),x);
    x = *it_x-GRID_SIZE;
    auto it_y = lower_bound(grids.begin(), grids.end(),y);
    y = *it_y-GRID_SIZE;
    setColor(color);
    glRecti(x, y+1, x+GRID_SIZE-1, y+GRID_SIZE);
    glFlush();
}

void drawGrids(int GRID_SIZE){
    setColor("black");
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
    setColor("white");

    //pre-processing
    for(int i=0; i<DIM; i=i+GRID_SIZE){
        grids.push_back(i);
        // cout<<i<<" ";
    }
    // cout<<"\n";

    // draw grids and its boundaries
    drawGrids(GRID_SIZE);

    glFlush();
}

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
        drawBlock(xi, yi, "black");     //******obstacles******
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