#include <cstdio>
#include <gl/glew.h>
#include <SOIL.h>
#include <gl/glut.h>
#include <gl/GLU.h>


class Point3D{
	public:
		Point3D(){};
		Point3D(float _x,float _y, float _z,float _roate,float _self_Roate):x(_x),y(_y),z(_z),rotate(_roate),self_Roate(_self_Roate){};
		float x,y,z,rotate,self_Roate;
		float getx(){
			return x;
		}
		float gety(){
			return y;
		}
		float getz(){
			return z;
		}
};

Point3D EARTH(600.0f,0.0f,0.0f,0.0f,5.0f);
Point3D MOON(400.0f,0.0f,0.0f,0.0f,5.0f);

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "SOIL.lib")
GLuint texture_Planet[3];
GLuint texture_earth;
GLUquadricObj *quadPlanet;
GLuint texture_SUN;
GLUquadricObj *quadSun;
GLuint texture_moon;
GLUquadricObj *quadMoon;
GLuint texture_background[1];
GLUquadricObj *quadBackground;

void Initial3(){
    glewInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	int earthHeight, earthWidth,sunWidth,sunHeight,moonWidth,moonHeight,backgroundWidth,backgroundHeight;
    unsigned char *earth = SOIL_load_image("C:/Users/dell/Desktop/EARTH.jpg", &earthWidth, &earthHeight, 0, SOIL_LOAD_RGB);
	unsigned char *sun = SOIL_load_image("C:/Users/dell/Desktop/sun.jpg", &sunWidth, &sunHeight, 0, SOIL_LOAD_RGB);
	unsigned char *moon=SOIL_load_image("C:/Users/dell/Desktop/MOON.jpg", &moonWidth, &moonHeight, 0, SOIL_LOAD_RGB);
	unsigned char *background=SOIL_load_image("C:/Users/dell/Desktop/maxresdefault.jpg", &backgroundWidth, &backgroundHeight, 0, SOIL_LOAD_RGB);
    glGenTextures(3, texture_Planet);
	glBindTexture(GL_TEXTURE_2D, texture_Planet[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, earthWidth, earthHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earth);
	glBindTexture(GL_TEXTURE_2D, texture_Planet[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sunWidth, sunHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sun);
	glBindTexture(GL_TEXTURE_2D, texture_Planet[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, moonWidth, moonHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, moon);
	
	glGenTextures(1,texture_background);
	glBindTexture(GL_TEXTURE_2D, texture_background[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, backgroundWidth, backgroundHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, background);

    SOIL_free_image_data(earth);
	SOIL_free_image_data(sun);
	SOIL_free_image_data(moon);
	SOIL_free_image_data(background);

    quadPlanet = gluNewQuadric();
	quadSun=gluNewQuadric();
	quadMoon=gluNewQuadric();
	quadBackground =gluNewQuadric();
}


void Reshape(GLsizei w,GLsizei h)  
{  
    if(h<0)  
    {  
        h=1;  
    }  
    glViewport(0,0,w,h);  
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  
    glOrtho(-800,800,-800,800,-800,800);  
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();  
}

void drawMoon(Point3D Point,double angle)
{
	glPushMatrix();
		glRotatef(45,1.0f,0.0f,0.0f);
		glTranslatef(Point.x,Point.y,Point.z);;
		glRotatef(MOON.rotate,0.0f,0.0f,1.0f);
		glTranslatef(0.0f, 100.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, texture_Planet[2]);
		glGenerateMipmap(GL_TEXTURE_2D);
		gluQuadricTexture(quadMoon, GLU_TRUE);
		gluSphere(quadMoon, 25, 50, 50);

		MOON.rotate += 1.0f;
		if(MOON.rotate>360.0f)
			MOON.rotate = 0.0f;
	glPopMatrix();
}

void drawSun(double angle)
{
	glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture_Planet[1]);
    glGenerateMipmap(GL_TEXTURE_2D);
    gluQuadricTexture(quadSun, GLU_TRUE);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    gluSphere(quadSun, 100, 50, 50);
	glPopMatrix();
}

void drawEarth(Point3D Point,double angle)
{
	glPushMatrix();
	glRotatef(45,0.0f,1.0f,1.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
	drawMoon(Point,angle);
	//glRotatef(45,0.0f,0.0f,1.0f);
	glTranslatef(Point.x, Point.y, Point.z);
	glBindTexture(GL_TEXTURE_2D, texture_Planet[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    gluQuadricTexture(quadPlanet, GLU_TRUE);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    gluSphere(quadPlanet, 50, 50, 50);

	glPopMatrix();
}

void setLight(void)
{
    static const GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const GLfloat light_ambient[]  = {3.0f, 3.0f, 3.0f, 1.0f};
    static const GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
void Background()
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture_background[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTranslatef(0.0f,0.0f,-790.0f);
	glScalef(800.0f,800.0f,800.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, -1.0);
	glTexCoord2d(1.0, 0.0); glVertex2d(+1.0, -1.0);
	glTexCoord2d(1.0, 1.0); glVertex2d(+1.0, +1.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, +1.0);
	glEnd();
	glPopMatrix();
}


void RenderScene(void) {
    static float angle = 0.0f;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	setLight();
	Background();
	drawEarth(EARTH,angle);
	drawSun(angle);
    angle += 0.1f;
    if (angle > 360.0f)
        angle = 0.0f;
	
    glutSwapBuffers();
}

void TimerFunc(int value){
    glutPostRedisplay();
    //glutTimerFunc(100, TimerFunc, 1);
}
void idle()
{
	glutPostRedisplay();
}
int main(int argc,char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
    glutCreateWindow("sphereTexture");

	glutReshapeFunc(Reshape);
    glutDisplayFunc(RenderScene);
    glutIdleFunc(idle);
    Initial3();
    glutMainLoop();

    return 0;
}