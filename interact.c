

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>

// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;

// Specity the values to place and size the window on the screen

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;

const int WINDOW_MAX_X = 500;
const int WINDOW_MAX_Y = 500;

int SCALABLE_WINDOW_MAX_X = 500;
int SCALABLE_WINDOW_MAX_Y = 500;

// Specify the coordinate ranges for the world coordinates in the 2D Frame

float WORLD_COORDINATE_MIN_X = 200.0;
float WORLD_COORDINATE_MAX_X = 2400.0;
float WORLD_COORDINATE_MIN_Y = 200.0;
float WORLD_COORDINATE_MAX_Y = 2400.0;

float scaleX, scaleY; // Used to properly scale viewport undergoing a change

/*** END of variables with Global scope ***/


void myglutInit( int argc, char** argv )
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(WINDOW_MAX_X,WINDOW_MAX_Y); /* set pixel window */
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y); /* place window top left on display */
    glutCreateWindow("Mouse and Keyboard Interaction"); /* window title */
}


void myInit(void)
{
 
/* standard OpenGL attributes */

      glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
      glColor3f(1.0, 0.0, 0.0); /* draw in red */
      glPointSize(10.0);

      COLORS_DEFINED = 0;

/* set up viewing window with origin lower left */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(WORLD_COORDINATE_MIN_X, WORLD_COORDINATE_MAX_X,
                 WORLD_COORDINATE_MIN_Y, WORLD_COORDINATE_MAX_Y);
      glMatrixMode(GL_MODELVIEW);
}

void reshape ( int newWidth, int newHeight ) {

      float SCALED_MAX_X, SCALED_MAX_Y;

      scaleX = (float) newWidth / (float) WINDOW_MAX_X;
      scaleY = (float) newHeight / (float) WINDOW_MAX_Y;

      printf ("In reshape with scaleX = %f, scaleY = %f ", scaleX, scaleY ); 
         
      SCALED_MAX_X = WORLD_COORDINATE_MIN_X + 
                       scaleX * (WORLD_COORDINATE_MAX_X - WORLD_COORDINATE_MIN_X);
 
      SCALED_MAX_Y = WORLD_COORDINATE_MIN_Y + 
                       scaleY * (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);

      printf ("New MaxX = %f, New MaxY = %f\n", SCALED_MAX_X, SCALED_MAX_Y ); 

      glMatrixMode( GL_PROJECTION );
      glLoadIdentity ();

      printf("Calling gluOrtho2D with %f, %f, %f, %f\n",
                     WORLD_COORDINATE_MIN_X, SCALED_MAX_X,
                     WORLD_COORDINATE_MIN_Y, SCALED_MAX_Y);

      gluOrtho2D(WORLD_COORDINATE_MIN_X, SCALED_MAX_X,
                 WORLD_COORDINATE_MIN_Y, SCALED_MAX_Y);

      glMatrixMode(GL_MODELVIEW);

      glClear (GL_COLOR_BUFFER_BIT);
      SCALABLE_WINDOW_MAX_X = newWidth;
      SCALABLE_WINDOW_MAX_Y = newHeight; 

}

void display( void )
{

/* define a point data type */

    typedef GLfloat point[2];     

    point p; /* A point in 2-D space */

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

    /* define point */

    p[0] = 100; 
    p[1] = 100;
   
    /* plot new point */

        glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd();
  
     
     glFlush(); /* clear buffers */

 }


void drawBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3ub( red, green, blue );

    // Added code to print window coordinates and world coordinates
    
    printf ("%d   %d (window coordinates) ", x, SCALABLE_WINDOW_MAX_Y - y );

    // I switch the mouse coordinate below and...

    p[0] = x;
    p[1] = SCALABLE_WINDOW_MAX_Y - y;  

    // then use the scaling equations we talked about in class...

    // Note, since x and y have to be between 0 and WINDOW_MAX_X and WINDOW_MAX_Y respectively, the coding
    // is actually a little simpler. 

    p[0] = WORLD_COORDINATE_MIN_X +  p[0] / WINDOW_MAX_X * 
                                    (WORLD_COORDINATE_MAX_X - WORLD_COORDINATE_MIN_X);


    p[1] = WORLD_COORDINATE_MIN_Y +  p[1] / WINDOW_MAX_Y * 
                                    (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);

   
    printf ("\t  %f   %f (world coordinates) \n", p[0], p[1] );

    glBegin(GL_POINTS);
    glVertex2fv(p); 
    glEnd();
    glFlush();
}


void eraseBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3f( 1.0, 1.0, 1.0 );

    p[0] = x;
    p[1] = y;  

    glBegin(GL_POINTS);
      glVertex2fv(p); 
    glEnd();
    glFlush();
}


void clearBox()
{
       glClear(GL_COLOR_BUFFER_BIT); 
       glFlush();
}


void mouse( int button, int state, int x, int y )
{ 

  if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        drawBox( x, y );
     }

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        eraseBox( x, y );
     }
  
  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        clearBox();
     }
}


void keyboard( unsigned char key, int x, int y )
{ 
  if ( key == 'q' || key == 'Q') exit(0);
}


int main(int argc, char** argv)
{

    myglutInit(argc,argv); /* Set up Window */
    myInit(); /* set attributes */

    // Now start the standard OpenGL glut callbacks //
    
    glutMouseFunc(mouse);  /* Define Mouse Handler */
    glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
    glutReshapeFunc(reshape); /* This should call the function to rescale */
    glutDisplayFunc(display); /* Display callback invoked when window opened */
    glutMainLoop(); /* enter event loop */
}

