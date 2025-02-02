/***********************************************
   CTIS164 - Homework1
   Seyedmoein Faghih - 22301542 - Section002
   An animated bird moving left-right in a window.
   Additional functions:
		1. Not only the bird is moving left-right, its wings are also animated and moving up-down.
		2. The curtains can be pulled using the mouse right-click.
		3. Using the Up and Down arrows, the bird can be moved vertically.
		4. Using the Right and Left arrows, the speed can be adjusted.
		5. In the case that the speed reaches the Max or Min speed, an alert is shown on the screen.
		6. An algorithm was created for displaying the Break pattern of the wall.
************************************************/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 600
#define PI 3.1415

// Global variables 
int  winWidth, winHeight; 

int cntrX = -500, cntrY = 0, wingY = 24, direction = 0, TIMER_PERIOD = 10, speedStatus = 0;

bool activeTimer = true, curtain = false;



//To draw an oval-like shape
void drawEllipse(float X, float Y, float radiusX, float radiusY, int numSegments) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < numSegments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float x = radiusX * cosf(theta);
		float y = radiusY * sinf(theta);

		glVertex2f(x + X, y + Y);
	}
	glEnd();
}

void circle(int x, int y, int r) {
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void vprint(int x, int y, void* font, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}


//To draw the bird
void drawBird(int centerX, int centerY, int wingVertexY)
{
	//legs
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(centerX, centerY-2);
	glVertex2f(centerX - 90, centerY-2);
	glVertex2f(centerX, centerY+2);
	glVertex2f(centerX - 90, centerY+2);
	glEnd();

	//beak
	glBegin(GL_TRIANGLES);
	glVertex2f(centerX + 75, centerY-2);
	glVertex2f(centerX + 102, centerY-2);
	glVertex2f(centerX + 75, centerY+4);
	glEnd();

	//Body, Neck, and Head
	glColor3ub(255, 128, 128);
	glRectf(centerX, centerY + 3, centerX + 75, centerY - 3);
	drawEllipse(centerX, centerY, 35, 17, 800);
	circle(centerX + 75, centerY, 10);

	//Eye
	glColor3f(0, 0, 0);
	circle(centerX + 75, centerY+2, 2);

	//Wings
	glColor3ub(255, 204, 204);
	glBegin(GL_TRIANGLES);
	glVertex2f(centerX, centerY + 8);
	glVertex2f(centerX, centerY - 8);
	glVertex2f(centerX - 45, wingVertexY);
	glEnd();

}


void onKeyDown(unsigned char key, int x, int y) {
	// Exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// Pause the timer when spacebar is pressed.
	else if (key == ' ')
		activeTimer = !activeTimer;

	
	glutPostRedisplay();

}

void onClick(int button, int stat, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)
		curtain = !curtain; //Make the curtains come on and off using right-click.
}

void onSpecialKeyDown(int key, int x, int y) {
	// Moving the Bird vertically.
	if (key == GLUT_KEY_DOWN)
		if (cntrY > -200)
			cntrY -= 5;

	if (key == GLUT_KEY_UP) 
		if (cntrY < 200)
			cntrY += 5;

	//Adjusting the speed.
	if (key == GLUT_KEY_RIGHT) {
		if (TIMER_PERIOD > 3) {
			TIMER_PERIOD--;  //Increase the speed.
			speedStatus = 0; //indicate that we are neither at min nor at max speed.
		}
		else
			speedStatus = 1; //shows that we are at Max Speed.
	}

	if (key == GLUT_KEY_LEFT) {
		if (TIMER_PERIOD < 30) { 
			TIMER_PERIOD++; //Decrease the speed.
			speedStatus = 0; //indicate that we are neither at min nor at max speed.
		}
		else
			speedStatus = -1; //shows that we are Min Speed.

	}

}



void display() {
	//The Program Window
	glClearColor(0.70, 0.35, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	//The Blue Window
	glColor3f(0.5, 0.74, 1);
	glRectf(-400, -200, 400, 200);

	//The Bird
	drawBird(cntrX, cntrY, wingY);

	//Background
	glColor3f(0.70, 0.35, 0);
	glRectf(-640, 300, -400, -300);
	glRectf(640, 300, 400, -300);
	glRectf(-400, -200, 400, -300);
	glRectf(-400, 200, 400, 300);
	

	//The Wall Pattern
	//To draw the Break Patterns correctly and only on the wall, avoiding drawing on the window.

	//Horizontal Lines
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	for (int i = WINDOW_HEIGHT / 2; i > -WINDOW_HEIGHT / 2; i -= 40) {
		if (i > -200 && i < 200)
		{
			glVertex2f(-WINDOW_WIDTH / 2, i);
			glVertex2f(-400, i);
			glVertex2f(400, i);
			glVertex2f(+WINDOW_WIDTH / 2, i);
		}
		else
		{
			glVertex2f(-WINDOW_WIDTH / 2, i);
			glVertex2f(+WINDOW_WIDTH / 2, i);
		}
	}

	//Vertical Lines, which are the trickiest part, since the lines need to be drawn once every second, and in special order.
	for (int i = -320; i < 400; i += 160)
	{
		for (int j = 300; j > -300; j -= 80)
		{
			if ((j > 200 && j - 40 > 200) || j < -200) {
				glVertex2f(i, j);
				glVertex2f(i, j - 40);
			}
			else if (j > 200 && j - 40 < 200)
			{
				glVertex2f(i, j);
				glVertex2f(i, 200);
			}
			else if (j >-200 && j-40<-200)
			{
				glVertex2f(i, j-40);
				glVertex2f(i, -200);
				
			}
		}
	}

	for (int i = -240; i < 400; i += 160)
	{
		for (int j = 260; j > -300; j -= 80)
		{
			if (j > 200 || j<-200)
			{
				glVertex2f(i, j);
				glVertex2f(i, j - 40);
			}
		}
	}


	int j = WINDOW_HEIGHT / 2;
	for (int i = -WINDOW_WIDTH / 2; i <= -400; i += 160)
	{
		while (j > -WINDOW_HEIGHT / 2)
		{
			glVertex2f(i, j);
			glVertex2f(i, j - 40);
			j -= 80;
		}
		j = WINDOW_HEIGHT / 2 - 40;
		while (j > -WINDOW_HEIGHT / 2)
		{
			glVertex2f(i + 80, j);
			glVertex2f(i + 80, j - 40);
			j -= 80;
		}
		j = WINDOW_HEIGHT / 2;
	}

	j = WINDOW_HEIGHT / 2 - 40;
	for (int i = 400; i < 600; i += 160)
	{
		while (j > -WINDOW_HEIGHT / 2)
		{
			glVertex2f(i, j);
			glVertex2f(i, j - 40);
			j -= 80;
		}
		j = WINDOW_HEIGHT / 2;
		while (j > -WINDOW_HEIGHT / 2)
		{
			glVertex2f(i + 80, j);
			glVertex2f(i + 80, j - 40);
			j -= 80;
		}
		j = WINDOW_HEIGHT / 2 -40;
	}
	glEnd();

	//Window Border
	glColor3ub(77, 38, 0);
	glRectf(-411, 211, -389, -211);
	glRectf(-411, 211, 411, 189);
	glRectf(411, 211, 389, -211);
	glRectf(389, -211, -411, -189);

	//Window Handle
	glColor3ub(255, 255, 102);
	circle(400, 0, 7);
	glRectf(397, 0, 403, -30);

	//Label
	glColor3ub(25, 51, 102);
	glRectf(-270, 280, 270, 220);
	glColor3ub(255, 255, 255);
	vprint(-252, 240, GLUT_BITMAP_TIMES_ROMAN_24, "CTIS 164 Homework1 done by Seyedmoein Faghih");

	//Curtain
	if (curtain) {
		glColor3ub(107, 107, 71);
		glRectf(-389, 189, 389, -189);
	}

	//Speed Status Error/Alert
	switch (speedStatus)
	{
	case 1:	glColor3ub(255, 0, 0); //Max Speed
		drawEllipse(500, 0, 80, 50, 400);
		glColor3f(0, 0, 0);
		vprint(434, -6, GLUT_BITMAP_TIMES_ROMAN_24, "MAX SPEED");
		break;
	case -1:	glColor3ub(255, 0, 0); //Min Speed
		drawEllipse(500, 0, 80, 50, 400);
		glColor3f(0, 0, 0);
		vprint(434, -6, GLUT_BITMAP_TIMES_ROMAN_24, "MIN SPEED");
		
	}

	//The Guide
	glColor3f(1, 1, 1);
	glRectf(413, -175, 635, -295);

	glColor3f(0, 0, 0);
	vprint(415, -190, GLUT_BITMAP_8_BY_13, "Right Arrow->Increase Speed");
	vprint(415, -210, GLUT_BITMAP_8_BY_13, "Left Arrow->Decrease Speed");
	vprint(415, -230, GLUT_BITMAP_8_BY_13, "Up and Down Arrows");
	vprint(451, -250, GLUT_BITMAP_8_BY_13, "-> Move Up and Down");
	vprint(415, -270, GLUT_BITMAP_8_BY_13, "Spacebar -> Pause");
	vprint(415, -290, GLUT_BITMAP_8_BY_13, "Right-Click -> Curtains");

	glutSwapBuffers();
}


void onResize(int w, int h) {
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); 
}


void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (activeTimer) {
		cntrX += 1; //Move the birds horizontally 
		
		
		switch (direction)  //this variable indicates wether the wings are moving upwards(1), or downwards(2).
		{
		case 0:	 //Wings going downwards
			if (wingY < cntrY-35)  //check to see if the wings are at the lowest point
		{
			direction = 1;  //changes the direction to upwards
			wingY += 1.3;	//Moves the wings vertically
		}
			  else
			wingY -= 1.3;	//continues the movement downwards
			break;
		case 1:  //Wings going upwards
			if (wingY > cntrY+35)	//check to see if the wings are at the highest point
			{
				direction = 0;	//changes the direction to downwards
				wingY -= 1.3;	//Moves the wings vertically
			}
			else
				wingY += 1.3;	//continues the movement upwards
		}

		if (cntrX > 560)	//Start the bird animation again from the beging after it reaches the end.
			cntrX = -500;
		
	}

	
	glutPostRedisplay(); 
}


void init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(40, 40);
	glutCreateWindow("Moein Faghih");

	// Window Events
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//Keyboard Event
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	//Mouse Event
	glutMouseFunc(onClick);

	//Timer Event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	init();
	glutMainLoop();
	return 0;
}