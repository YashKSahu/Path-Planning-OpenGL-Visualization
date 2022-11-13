// OpenGL essentials, header files
#include <GL/glut.h>

// header file to include all C++ headers
#include <bits/stdc++.h>
using namespace std;

// header file for path planning algorithm
#include "path_planning.hpp"

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// variable to track mouse location
int onMouse;

// int HEIGHT=720,WIDTH=720;
int DIM=720;
int GRID_BLOCK_SIZE=30;
int GRID_SIZE = DIM/GRID_BLOCK_SIZE;

// variables to set start and goal status
// whether start and goal drawn or not
bool START=false;
bool GOAL=false;

// vector pair to hold start and goal indices
Pair start,goal;

// variable to check search attempt
bool find_attempt = false;

// list of pixel grids start locations
vector<int> pixel_grids;

// matrix of grid indices
vector<vector<int>> grid(GRID_SIZE , vector<int> (GRID_SIZE, 0));

// Opengl initialization function
void init(){
    glClearColor(1,1,1,0);  //white background
    // glLoadIdentity();
    // gluOrtho2D(0, 640, 0, 480);
    glOrtho(0, DIM, 0, DIM,-1,1);
}

// Function to set pixel color
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

// Function to get clicked pixel color
vector<float> getPixelColor(int x, int y){
    vector<float> pixel_color(3);
    float color[3];
    glReadPixels(x,DIM-y,1.0,1.0,GL_RGB,GL_FLOAT,color);
    for(int i=0; i<=2; ++i)
        pixel_color[i]=color[i];
    return pixel_color;
}

// Function to draw a point
void pointPlot(int x, int y, int center_x, int center_y){
    glPointSize(2);
    glBegin(GL_POINTS);
        glVertex2i(x+center_x,y+center_y);
    glEnd();
}

// Function to draw line
void drawLine(int x1,int y1, int x2, int y2){
    glLineWidth(1.0);
    glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
    glEnd();
}

// Function to draw rectangle
//Input = grid indices
void drawRect(int x,int y){
    int x1=x*GRID_BLOCK_SIZE+1;
    int y1=y*GRID_BLOCK_SIZE+1;
    int x2=x1+GRID_BLOCK_SIZE-3;
    int y2=y1+GRID_BLOCK_SIZE-3;
    
    glRecti(x1, DIM-(y1), x2, DIM-(y2));
    glFlush();
}

// Function to print grid
void printGrid(){
    cout<<"\n-------------------\n";
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++)
            cout << grid[i][j] << " ";
        cout<< endl;
    }
    cout<<"-------------------\n";
}

// Function to introduce delay in seconds
void sleep(float seconds){
    int time = 1000*seconds;
    this_thread::sleep_for(chrono::milliseconds(time));
}

// Function to find path
void findPath(Pair start, Pair goal){

    if(!START or !GOAL){
        cout<<"Select a START or GOAL!\n";
        return;
    }
    // cout<<start.first<<" "<<start.second<<"\n";
    // cout<<goal.first<<" "<<goal.second<<"\n";

    /* Description of the Grid-
    1--> The cell is not blocked
    0--> The cell is blocked */

    //intialize array
    //copying vector data to array,
    //path planning algo, requires array data type
    array<array<int, 24>, 24> grid_array;
    for(int i = 0; i < GRID_SIZE; i++)
        for(int j = 0; j < GRID_SIZE; j++)
            grid_array[i][j]=grid[i][j];

    // calling path planning algorithm
    aStarSearch(grid_array, start, goal);

    //plotting found path
    setColor("green");
    for(int i=1; i<maze_path.size()-1; ++i){
        // printf("-> (%d,%d) ", maze_path[i].first, maze_path[i].second);
        drawRect(maze_path[i].second, maze_path[i].first);
        glFlush();
        sleep(0.1);
    }
    // cout<<"\n";

    //terminate message
    find_attempt = true;
    cout<<"Path Plotted, please terminate the program.\n";
}

// Function to draw a block on mouse click
void drawBlock(int x, int y, string color){

    //Variables x and y hold the clicked pointer location

    //Get pixel color
    vector<float> pixel_color = getPixelColor(x,y);
    // cout<<pixel_color[0]<<" "<<pixel_color[1]<<" "<<pixel_color[2]<<" \n";

    //If Green = find path
    if(round(pixel_color[0])==0 and round(pixel_color[1])==1 and round(pixel_color[2])==0)
        findPath(start, goal);

    //If not White = already colored, return
    if(round(pixel_color[0])!=1 or round(pixel_color[1])!=1 or round(pixel_color[2])!=1) return;

    int grid_x=y/GRID_BLOCK_SIZE;
    int grid_y=x/GRID_BLOCK_SIZE;
    
    //Draw start and goal block first
    if(START==false){
        START=true;
        start.first=grid_x;
        start.second=grid_y;
        color = "blue";     //******start*****
    }
    else if(GOAL==false){
        goal.first=grid_x;
        goal.second=grid_y;
        GOAL=true;
        color = "red";      //******goal*****
    }

    //Get the lower bounds of x and y, to fit the drawn block with grid's block
    auto it_x = lower_bound(pixel_grids.begin(), pixel_grids.end(),x);
    x = *it_x-GRID_BLOCK_SIZE;
    auto it_y = lower_bound(pixel_grids.begin(), pixel_grids.end(),y);
    y = *it_y-GRID_BLOCK_SIZE;
    // cout<<x<<" "<<y<<" - "<<*it_x<<" "<<*it_y<<"\n";

    //Draw Block
    setColor(color);
    glRecti(x+1, DIM-(y+1), x+GRID_BLOCK_SIZE-2, DIM-(y+GRID_BLOCK_SIZE-2));
    glFlush();
    
    //Update GRID
    grid[grid_x][grid_y]=0;
    if(color=="blue" or color=="red") grid[grid_x][grid_y]=1;

    // printGrid();
}

// Function to draw grid lines and obstacle wall around the grid
void drawGrids(int GRID_BLOCK_SIZE){
    setColor("black");
    for(int i=0; i<=DIM-GRID_BLOCK_SIZE; i=i+GRID_BLOCK_SIZE){
        drawLine(i,0,i,DIM);
        drawLine(0,i,DIM,i);
        
        // Grid Boundaries
        glRecti(i, 0, i+GRID_BLOCK_SIZE, GRID_BLOCK_SIZE);
        glRecti(i, DIM-GRID_BLOCK_SIZE, i+GRID_BLOCK_SIZE, DIM);
        glRecti(0, i, GRID_BLOCK_SIZE, i+GRID_BLOCK_SIZE);
        glRecti(DIM-GRID_BLOCK_SIZE, i, DIM, i+GRID_BLOCK_SIZE);
    }
}

// Function to display text on screen
void drawText(int x, int y, float r, float g, float b, string text){
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = text.length();
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
  }
}

// Main Rendering function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    setColor("white");

    //pre-processing
    for(int i=0; i<DIM; i=i+GRID_BLOCK_SIZE){
        pixel_grids.push_back(i);
        // cout<<i<<" ";
    }
    // cout<<"\n";

    // draw grids and its boundaries
    drawGrids(GRID_BLOCK_SIZE);

    // draw path plot button, top-middle
    setColor("green");
    int button_origin_x = DIM/2;
    int button_origin_y = DIM;
    int offset=GRID_BLOCK_SIZE/10;
    int button_length = 4*GRID_BLOCK_SIZE;
    int button_width = 1*GRID_BLOCK_SIZE;
    glRecti(button_origin_x - button_length/2, button_origin_y, button_origin_x + button_length/2, button_origin_y - button_width + offset);

    // add label to path plot button
    setColor("red");
    int label_x=DIM/2-1.5*GRID_BLOCK_SIZE;
    int label_y=DIM-3*GRID_BLOCK_SIZE/4;
    drawText(label_x,label_y,1,0,0,"Plot Path");

    // Initializing Grid
    for(int i=1; i<GRID_SIZE-1; ++i)
        for (int j=1; j<GRID_SIZE-1; ++j)
            grid[i][j]=1;

    // setColor("red");
    // drawRect(GRID_SIZE-1,GRID_SIZE-1);

    glFlush();
}

// Function to track and map mouse clicks to actions
void mouseClick(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int xi = x;
        int yi = y;
        if(!find_attempt)
            drawBlock(xi, yi, "black");     //******obstacles******
    }
}

// Function main
int main(int argc, char** argv){
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