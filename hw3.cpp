
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>
#include <iostream>
#include  <cstdlib>

/*============================================================================*/

using namespace std;
#define NUM_STEPS 1000

int globali       = 0;
int flag          = 0;
int extraFlag     = 0;
int runs          = 1;
int count         = 0;
int countBlend    = 0;
int newinput      = 0;
int numPoints     = 0;
int divisioncount = 0;
int castel        = 0;
int sub           = 0;
int blending      = 0;
int opengl        = 0;
int patch         = 0; 
GLfloat pts[80][3];
GLfloat gx;
GLfloat gy;

/*============================================================================*/

struct point{
  GLfloat x;
  GLfloat y;
};

/*============================================================================*/

point Bezier[100000];
point points[80];

GLfloat PatchPoints[4][4][3] = {
   {{-1.5, -1.5, 2.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 

   {{-1.5, -0.5, -1.0}, {-0.5, 0.5, 3.0}, 
    {1.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 

   {{-1.5, -0.5, -1.0}, {-0.5, 0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {4.5, -0.5, -1.0}}, 
   /*
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, */

     {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
       {0.5, 3.5, 0.0}, {1.5, 3.5, -1.0}}
 
};


/*============================================================================*/

void surface_patch(void)
{
  int i, j;

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
	  0, 1, 12, 4, &PatchPoints[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix ();
  glRotatef(85.0, 1.0, 1.0, 1.0);
  for (j = 0; j <= 100; j++) {
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 50; i++)
      glEvalCoord2f((GLfloat)i/50.0, (GLfloat)j/100.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 50; i++)
      glEvalCoord2f((GLfloat)j/100.0, (GLfloat)i/50.0);
    glEnd();
  }
  glPopMatrix ();
  // glFlush();
}

/*============================================================================*/

void open_gl(){
  newinput = 0;
  int i = 0,k=0,m,inc;
  glClear(GL_COLOR_BUFFER_BIT);
  
  if(flag){
    inc = 1;
    flag = 0;
  } else {
    inc = 0;
  }
  for(m=0;m<numPoints;m++){
    pts[m][0]=points[m].x;
    pts[m][1]=points[m].y;
    pts[m][2]=0;
  }

  for(k=0;k<runs /*+ inc*/;k++) {       
    
    glMap1f(GL_MAP1_VERTEX_3,0.0,1.0,3,4, &pts[3*k][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= NUM_STEPS; i++) {
      glEvalCoord1f((GLfloat)i/NUM_STEPS);
    } 
    glEnd();    
  }
  //  glFlush();
}

/*============================================================================*/

void get_pts(point array[],point  l[],point  r[],int loc){
  point q[4];
  int i,j,k;
  GLfloat u = 0.5;

  point lnew[4];
  point rnew[4];

  l[0].x = array[0].x;
  l[0].y = array[0].y;
  
  r[3].x = array[3].x;
  r[3].y = array[3].y;


  for(i=0;i<4;i++) {
    q[i].x = array[i].x;
    q[i].y = array[i].y;
  }

  for(k=1;k<4;k++){
    for(i=0;i<4-k;i++) {
      if(k == 3){
	l[2].x = q[i].x;
	l[2].y = q[i].y;
	
	r[1].x = q[i+1].x;
	r[1].y = q[i+1].y;  
      }
      q[i].x = (1 - u) * q[i].x + u*q[i + 1].x;
      q[i].y = (1 - u) * q[i].y + u*q[i + 1].y;
      
      if(k==1 && i==0){
	l[1].x = q[i].x;
	l[1].y = q[i].y; 
      } else if(k==1 && i==2){
	r[2].x = q[i].x;
	r[2].y = q[i].y; 
      } else if( k == 3) {
	l[3].x = r[0].x = q[i].x;
	l[3].y = r[0].y = q[i].y; 
      }
    }
  }

 
  

  if(loc == divisioncount){
 glBegin(GL_LINE_STRIP);
  for (i = 0; i < 4; i++) {
    glVertex2f(l[i].x,l[i].y);
  }
  glEnd();
  
  glBegin(GL_LINE_STRIP);
  for (i = 0; i < 4; i++) {
    glVertex2f(r[i].x,r[i].y);
  }
  glEnd();
    return;
  }
  get_pts(l,lnew,rnew,loc + 1);
  get_pts(r,lnew,rnew,loc + 1);
}

/*============================================================================*/

void subdivision(){
  point *arr;
  point l[4];
  point r[4];
  int i;
  for(i=0;i<runs;i++){
    arr = &points[0+3*i];
      get_pts(arr,l,r,0);
  }
  
  //get_pts(points,l,r,0);
  int y=0;
}

/*============================================================================*/

void blending_matrix(point po[], GLfloat u){
  GLfloat thepoint[3];  
  point q;
  point q1;
  int   k,m;
  int   inc;
  int limit;
  point p[numPoints];
  GLfloat temp;


for(m=0;m<numPoints;m++){
    p[m].x=points[m].x;
    p[m].y=points[m].y;
  }
    

 
  inc = 3*countBlend;
  limit = countBlend*1000 + globali;
  

  q.x = p[0 + inc].x*1 * 1   +  (p[0+ inc].x * -3 + p[1+ inc].x * 3 )* u   +   (p[0+ inc].x*3 + p[1+ inc].x*-6 + p[2+ inc].x*3)*u*u   +   (p[0+ inc].x*-1 + p[1+ inc].x*3+p[2+ inc].x*-3+p[3+ inc].x*1)*u*u*u;
  

  q.y = p[0 + inc].y*1 * 1   +  (p[0+ inc].y * -3 + p[1+ inc].y * 3 )* u   +   (p[0+ inc].y*3 + p[1+ inc].y*-6 + p[2+ inc].y*3)*u*u   +   (p[0+ inc].y*-1 + p[1+ inc].y*3+p[2+ inc].y*-3+p[3+ inc].y*1)*u*u*u;

 
  Bezier[limit].x = q.x;
  Bezier[limit].y = q.y;



  glPointSize(1.0);   	
  glBegin(GL_POINTS);

  for(k=0;k<limit;k++){
    thepoint[0] = Bezier[k].x;
    thepoint[1] = Bezier[k].y;
    thepoint[2] = 0;
    glColor3f(1.0+0.1, 0.5+0.5, 0.0+0.7*1);  
    glVertex3fv(thepoint); 
  }      
  glEnd();

  
}

/*============================================================================*/

void de_castelajau(point p[], GLfloat u){
  point q[4];
  int i,n=4,k;
  int y =0 ,j = 0,inc=0;
 
  inc = 3*count;
 
  for(i=0;i<n;i++) {
    q[i].x = p[i+inc].x;
    q[i].y = p[i+inc].y;
  }
  GLfloat thepoint[3];  
  for(k=1;k<n;k++){
    for(i=0;i<n-k;i++) {
      q[i].x = (1 - u) * q[i].x + u*q[i + 1].x;
      q[i].y = (1 - u) * q[i].y + u*q[i + 1].y;
      
      for(j=0;j<4 - 1;j++){
	if(k==1){
	  glColor3f(0.5+0.2, 0.26+0.3, 0.883+0.2);      
	} else if(k==2){
	  glColor3f(0.5-0.2, 0.26*0.3, 0.983-0.2);      
	} else if(k==3){
	  glColor3f(0.7, 0.3, 0.389);      
	}
	glBegin(GL_LINE_STRIP);
	glVertex2f(q[j].x,q[j].y);   
	glVertex2f(q[j+1].x,q[j+1].y);   
	glEnd();
      }
      
      
      if (k==4 -1) {
	thepoint[0] = q[0].x;
	thepoint[1] = q[0].y;
	thepoint[2] = 0;
	glBegin(GL_POINTS);
	glColor3f(1.0+0.1*i, 0.5+0.5*i, 0.0+0.7*i);  
	glVertex3fv(thepoint); //display a point   
	glEnd();

	Bezier[globali + 1000 * count].x = q[0].x;
	Bezier[globali + 1000 * count].y = q[0].y;
	
      }
    }
  }
  for(k=0;k<globali+(1000*count);k++){
    glBegin(GL_POINTS);
    thepoint[0] = Bezier[k].x;
    thepoint[1] = Bezier[k].y;
    thepoint[2] = 0;
    glPointSize(1.0);   	
    glColor3f(1.0+0.1*i, 0.5+0.5*1, 0.0+0.7*1);  
    glVertex3fv(thepoint); //display a point   
    glEnd();
  } 
}

/*============================================================================*/

void loop_function(){
  GLfloat thepoint[3];  
  int i;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0); //clear to black with alpha of 1
  
  glPointSize(5.0);   
  glBegin(GL_POINTS);
  for(i=0;i<numPoints;i++){
    thepoint[0] = points[i].x;
    thepoint[1] = points[i].y;
    thepoint[2] = 0;
    glColor3f(1.0+0.1*i, 0.5+0.5*1, 0.0+0.7*1);  
    glVertex3fv(thepoint); 
  }
  glEnd();

  if(numPoints<4 && !patch){
    if(numPoints == 1){
      gx = points[0].x;
      gy = points[0].y;
    }
    cout << "Please Enter " << 4-numPoints << " more point(s)" << endl;
    glFlush();
    return;
  }
  points[0].x = gx;
  points[0].y = gy;
  
  glPointSize(2.5);
  glColor3f(1.0, 1.0, 0.0);
     

  if(castel) {
    de_castelajau(points,(GLfloat)globali/1000);
  } else if(blending){
    blending_matrix(points,(GLfloat)globali/1000);
  } else if(sub){
    subdivision();
  } else if(opengl){
    open_gl();
    //return;
  } else if(patch){
    surface_patch();
  } 
  globali++;
  
  if(!patch){
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < numPoints; i++) {
      glVertex2f(points[i].x,points[i].y);
    }
    glEnd();
  }

 

 
  if (globali == 1001) {
    globali = 0;
      
    if(points[0].x==points[numPoints-1].x){
      int numsdfs = 0;
    }
    
      
    if(numPoints>4){
      if(blending){
	if(countBlend==runs - 1){
	  countBlend = 0;
	} else {
	  countBlend++;
	}
      } else {
	if(count == runs - 1){
	  count = 0;
	} else {
	  count++;
	}
      }
    }
  }
  glutSwapBuffers();
  glFlush();
}


/*============================================================================*/


void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
	    5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
  else
    glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
	    5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*============================================================================*/

void update_pts(GLfloat xin,GLfloat yin){
  int i;
  newinput =1;
  if(numPoints >= 4){
    flag = 1;
    int val = (4*runs - runs-1);
    if ((numPoints%val == 0 || numPoints%val == 3 || numPoints%val == 2)&&!extraFlag){
      GLfloat midx = (points[numPoints-1].x + points[numPoints -2].x)/2;
      GLfloat midy = (points[numPoints-1].y + points[numPoints-2].y)/2;
    
      for(i=numPoints-1 ; i<numPoints;i++){
	points[i+1].x = points[i].x;      
	points[i+1].y = points[i].y;      
      }
      points[numPoints-1].x = midx;
      points[numPoints-1].y = midy;
      numPoints++;
    
      points[numPoints].x = xin;
      points[numPoints].y = yin;
      numPoints++;
      points[numPoints].x = xin;
      points[numPoints].y = yin;
      extraFlag = 1;
      numPoints++;
      runs++;
    } else  if (extraFlag){
      points[numPoints-1].x = xin;
      points[numPoints-1].y = yin;
      extraFlag = 0;
    }
   
    globali=0;
    count=0;
    
  } else {
    if(numPoints<4){
      points[numPoints].x = xin;
      points[numPoints].y = yin;
      numPoints++;
      if(numPoints == 4){
	runs = 1;
      }
    }
  }
 
} 

/*============================================================================*/

void Keyboard(unsigned char key, int x, int y)
{
  GLfloat xin,yin;
  globali = 0; 
  if(key == 'b'){
    blending     = 1;    
    castel        = 0;
    sub           = 0;
    opengl        = 0;
    patch = 0; 
  } else if(key == 'o'){
    opengl        = 1;
    blending      = 0;
    castel        = 0;
    sub           = 0;
    patch = 0; 
  } else if(key == 's') {

    blending      = 0;
    castel        = 0;
    sub           = 1;
    opengl        = 0;
    patch = 0; 
  } else if (key == 'c') {

    blending      = 0;
    castel        = 1;
    sub           = 0;
    opengl        = 0;
    patch = 0; 

  } else if(key == 'p'){
    patch = 1; 
    opengl        = 0;
    blending      = 0;
    castel        = 0;
    sub           = 0;
  } else if(key == 'r'){
    numPoints = 0;
    runs = 1;
    globali       = 0;
    flag          = 0;
    extraFlag     = 0;
    runs          = 1;
    count         = 0;
    countBlend    = 0;
    newinput      = 0;
    numPoints     = 0;
    divisioncount = 0;
    castel        = 0;
    sub           = 0;
    blending      = 0;
    opengl        = 0;
    patch         = 0; 

  } else if((int)key == 27){
    exit(0);
  }
  
  if(key == '+'){
    divisioncount++;
  }
  
  if(key == '-'){
    if(divisioncount >= 1){
      divisioncount--;
    }
  }
}

/*============================================================================*/


void MouseMotion(int x, int y) {
  loop_function();
}

/*============================================================================*/

void AnimateScene(void)
{
  if(!opengl && !patch) {
     glutPostRedisplay();
  } else if(newinput){
    glutPostRedisplay();
    newinput = 0;
  }
}
 
/*============================================================================*/

/*
void init(void)
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
	  0, 1, 12, 4, &ctrlpoints[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
}
*/
/*============================================================================*/

int main1(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  //  init ();
  glutDisplayFunc(surface_patch);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}

/*============================================================================*/

void init_type1(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &pts[0][0]);
  glEnable(GL_MAP1_VERTEX_3);
}

/*============================================================================*/

void processMouse(int button, int state, int x, int y) {
  if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON ) {
   
    GLdouble modelMatrix[16];
    double position[3];
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    GLdouble projMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    gluUnProject(
		 x,
		 viewport[3] - y,
		 0,
		 modelMatrix,
		 projMatrix,
		 viewport,
		 //the next 3 parameters are the pointers to the final object
		 //coordinates. Notice that these MUST be double's
		 &position[0], //-&gt; pointer to your own position (optional)
		 &position[1], // id
		 &position[2] // id
		 );
    update_pts(position[0],position[1]);    
		 
  }
}

/*============================================================================*/

void speed_menu(GLint in) {
 
  char key = (char)in;

 globali = 0; 
  if(key == 'b'){
    blending     = 1;    
    castel        = 0;
    sub           = 0;
    opengl        = 0;
    patch = 0; 
  } else if(key == 'o'){
    opengl        = 1;
    blending      = 0;
    castel        = 0;
    sub           = 0;
    patch = 0; 
  } else if(key == 's') {

    blending      = 0;
    castel        = 0;
    sub           = 1;
    opengl        = 0;
    patch = 0; 
  } else if (key == 'c') {

    blending      = 0;
    castel        = 1;
    sub           = 0;
    opengl        = 0;
    patch         = 0; 

  } else if(key == 'p'){
    patch         = 1; 
    opengl        = 0;
    blending      = 0;
    castel        = 0;
    sub           = 0;
  } else if(key == 'r'){
    numPoints = 0;
    runs =1 ;
    numPoints = 0;
    runs = 1;
    globali       = 0;
    flag          = 0;
    extraFlag     = 0;
    runs          = 1;
    count         = 0;
    countBlend    = 0;
    newinput      = 0;
    numPoints     = 0;
    divisioncount = 0;
    castel        = 0;
    sub           = 0;
    blending      = 0;
    opengl        = 0;
    patch         = 0; 
  } else if((int)key == 27){
    exit(0);
  }

  if(key == '+'){
    divisioncount++;
  }
  
  if(key == '-'){
    if(divisioncount >= 1){
      divisioncount--;
    }
  }
}

/*============================================================================*/

int main(int argc,char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  glutInitWindowPosition(1200,25);
  glutInitWindowSize(512,512);
  glutCreateWindow("HW3 - Nitin Agrahara Ravikumar - 73714398");

 
  points[0].x = -4;
  points[0].y = 3;

  points[1].x = -2.5;
  points[1].y = -2.5;

  points[2].x = 2.5;
  points[2].y = -1.0;

  points[3].x = 3.0;
  points[3].y = 4.0;  
  
  init_type1();
  glutKeyboardFunc (Keyboard);
  glutDisplayFunc(loop_function);
  glutReshapeFunc(reshape);
  glutMouseFunc(processMouse);
  glutIdleFunc (AnimateScene);

  int models = 1;
  glutCreateMenu(speed_menu);
  glutAddMenuEntry("Options", 0);
  glutAddMenuEntry("", 0);
  glutAddMenuEntry("[c]   De Casteljau", 'c');
  glutAddMenuEntry("[b]   Blending Matrix", 'b');
  glutAddMenuEntry("[o]   OpenGL Methods", 'o');
  glutAddMenuEntry("[s]   Subdivision( + to increment and - to decrement)", 's');
  glutAddMenuEntry("[p]   Surface Patch", 'p');
  glutAddMenuEntry("[r]    Restart", 'r');
  glutAddMenuEntry("[ESC] Quit Program",  27);
  glutAddMenuEntry("", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);    
  glutMainLoop();
  return 0;
}

/*============================================================================*/
