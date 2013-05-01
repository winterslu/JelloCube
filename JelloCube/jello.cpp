/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

  Your name:
  Ke Lu

*/

#include "jello.h"
#include "showCube.h"
#include "input.h"
#include "physics.h"
#include "lighting.h"
#include "ActorPicking.h"

// camera parameters
double Theta = pi / 6;
double Phi = pi / 6;
double R = 6;

// Recording parameters
double timestamp;

// mouse control
int g_iMenuId;
int g_vMousePos[2];
int g_iLeftMouseButton,g_iMiddleMouseButton,g_iRightMouseButton;

// number of images saved to disk so far
int sprite=0;

// these variables control what is displayed on screen
int shear=0, bend=0, structural=1, pause=0, viewingMode=0, saveScreenToFile=0;

// 
int pushUp=0, pushDown=0, pushLeft=0, pushRight=0;

// TimeCounter
PerformanceCounter timeCounter;

struct world jello;

int windowWidth, windowHeight;

void myinit()
{
    
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0,1.0,0.01,1000.0);

  // set background color to grey
  glClearColor(0.5, 0.5, 0.5, 0.0);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
  
  // lighting initialization
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);


  return; 
}

void reshape(int w, int h) 
{
  // Prevent a divide by zero, when h is zero.
  // You can't make a window of zero height.
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Set the perspective
  double aspectRatio = 1.0 * w / h;
  gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 

  windowWidth = w;
  windowHeight = h;

  glutPostRedisplay();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // camera parameters are Phi, Theta, R
  gluLookAt(R * cos(Phi) * cos (Theta), R * sin(Phi) * cos (Theta), R * sin (Theta),
	        0.0,0.0,0.0, 0.0,0.0,1.0);


    // macro to set up light i
#define LIGHTSETUP(i)\
glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
glEnable(GL_LIGHT##i)
    
    LIGHTSETUP (0);
    //LIGHTSETUP (1);
    //LIGHTSETUP (2);
    //LIGHTSETUP (3);
    //LIGHTSETUP (4);
    //LIGHTSETUP (5);
    //LIGHTSETUP (6);
    //LIGHTSETUP (7);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
    //glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    
    // set up box color
    glMaterialfv(GL_FRONT, GL_AMBIENT, mBa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mBd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mBs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mBe);
    glMaterialf(GL_FRONT, GL_SHININESS, 20);
    // show the bounding box
    showBoundingBox();
    // show the inclined plane
    showInclinedPlane(&jello);
    
    
    // set up cube color
    glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
    glMaterialf(GL_FRONT, GL_SHININESS, 240);
    // show the cube
    showCube(&jello);
    
    
    
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 1.5);
    glEnd();
    
 
  glutSwapBuffers();
}

void doIdle()
{
  /* Calculate FPS */

  timeCounter.StopCounter();
  timestamp += timeCounter.GetElapsedTime();
  printf("FPS:%d\n",int(1/timeCounter.GetElapsedTime()));
  timeCounter.StartCounter();
    
  char s[20]="picxxxx.ppm";
  //int i;
  
  // save screen to file
  s[3] = 48 + (sprite / 1000);
  s[4] = 48 + (sprite % 1000) / 100;
  s[5] = 48 + (sprite % 100 ) / 10;
  s[6] = 48 + sprite % 10;

  if (saveScreenToFile==1 && timestamp >= 1/15)
  {
    saveScreenshot(windowWidth, windowHeight, s);
    //saveScreenToFile=0; // save only once, change this if you want continuos image generation (i.e. animation)
    sprite++;
    timestamp = 0;
  }

  if (sprite >= 300) // allow only 300 snapshots
  {
    exit(0);	
  }

  if (pause == 0)
  {
      //pause = 1;
      
      RK4(&jello);
      
      pushUp=pushDown=pushLeft=pushRight=0;

  }

  glutPostRedisplay();
}

int main (int argc, char ** argv)
{
  if (argc<2)
  {  
    printf ("Oops! You didn't say the jello world file!\n");
    printf ("Usage: %s [worldfile]\n", argv[0]);
    exit(0);
  }

  readWorld(argv[1],&jello);

  glutInit(&argc,argv);
  
  /* double buffered window, use depth testing, 640x480 */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);
  
  windowWidth = 640;
  windowHeight = 480;
  glutInitWindowSize (windowWidth, windowHeight);
  glutInitWindowPosition (300,200);
  glutCreateWindow ("Jello cube");

  /* tells glut to use a particular display function to redraw */
  glutDisplayFunc(display);

  /* replace with any animate code */
  glutIdleFunc(doIdle);

  /* callback for mouse drags */
  glutMotionFunc(mouseMotionDrag);

  /* callback for window size changes */
  glutReshapeFunc(reshape);

  /* callback for mouse movement */
  glutPassiveMotionFunc(mouseMotion);

  /* callback for mouse button changes */
  glutMouseFunc(mouseButton);

  /* register for keyboard events */
  glutKeyboardFunc(keyboardFunc);

  /* do initialization */
  myinit();

  /* forever sink in the black hole */
  glutMainLoop();

  return(0);
}

