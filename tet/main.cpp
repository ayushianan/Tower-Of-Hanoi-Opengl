#include <GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define LIGHT_ON 0
#define LIGHT_OFF 1
int pos[16] = {10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85};
int peg[3] = {50,150,250};
int moves[10000][3];
int max_moves;
int POLES[3][10];
int top[3]={-1,-1,-1};
int NUM_DISKS=3;
int cnt,counter,speed=5;
int line1=90,line2=85;
float ycoordinate;

void push(int p,int disk)
{
	POLES[p][++top[p]] = disk;
}

void pop(int p)
{
	top[p]--;
}

void tower(int n,int src,int temp,int dst)
{
	if(n>0)
	{
		tower(n-1,src,dst,temp);
		moves[cnt][0] = n;
		moves[cnt][1] = src;
		moves[cnt][2] = dst;
		cnt++;
		tower(n-1,temp,src,dst);
	}
}

void drawPegs()
{
	int i;
	glColor3f(0.5,0.0,0.1);
	for(i=0;i<3;i++)
	{
		glPushMatrix();
		glTranslatef(peg[i],5,0);
		glRotatef(-90,1,0,0);
		glutSolidCone(2,70,20,20);
		glutSolidTorus(2,45, 20, 20);
		glPopMatrix();
	}

}

void printString(char *text)
{
	int len=strlen(text),i;
	for(i=0;i<len;i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
}

void drawText()
{
	glColor3f(1,1,1);
	glRasterPos3f(-70,line1,0);
	printString("Move :");
	char str[5];
	sprintf(str, "%d", counter);
	glRasterPos3f(-40,line1,0);
	printString(str);
	glRasterPos3f(-70,line2,0);
	printString("Disk");
	char str1[10];
	sprintf(str1, "%d", moves[counter][0]);
	glRasterPos3f(-50,line2,0);
	printString(str1);
	glRasterPos3f(-40,line2,0);
	printString("from");
	char src[2];
	if(moves[counter][1]==0)strcpy(src,"A");
	else if(moves[counter][1]==1)strcpy(src,"B");
	else strcpy(src,"C");
	glRasterPos3f(-20,line2,0);
	printString(src);
	glRasterPos3f(-10,line2,0);
	printString("to");
	char dst[2];
	if(moves[counter][2]==0)strcpy(dst,"A");
	else if(moves[counter][2]==1)strcpy(dst,"B");
	else strcpy(dst,"C");
	glRasterPos3f(0,line2,0);
	printString(dst);
	glColor3f(0.6,0.3,0.5);
	glBegin(GL_POLYGON);
		glVertex3f(-75,93,-5);
		glVertex3f(-75,83,-5);
		glVertex3f(10,83,-5);
		glVertex3f(10,93,-5);
	glEnd();
	glColor3f(1,0,0);
	glRasterPos3f(peg[0],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'A');
	glRasterPos3f(peg[1],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'B');
	glRasterPos3f(peg[2],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'C');
}

void drawSolved()
{
	glColor3f(1,1,0);
	glRasterPos3f(-60,87,0);
	printString("Solved !!");
	glColor3f(0.6,0.3,0.5);
	glBegin(GL_POLYGON);
		glVertex3f(-75,93,-5);
		glVertex3f(-75,83,-5);
		glVertex3f(10,83,-5);
		glVertex3f(10,93,-5);
	glEnd();
	glColor3f(1,0,0);
	glRasterPos3f(peg[0],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'A');
	glRasterPos3f(peg[1],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'B');
	glRasterPos3f(peg[2],70,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'C');
}

void display()
{
	int i,j,k;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(0,ycoordinate,0,0,0,-1,0,1,0);
	drawPegs();
	for(i=0;i<3;i++)
	{
		k=0;
		for(j=0;j<=top[i];j++)
		{
			glPushMatrix();
			glTranslatef(peg[i],pos[k++],0);
			glRotatef(90,1,0,0);
			glColor3f(0.1*POLES[i][j],0.2*POLES[i][j],0);
			glutSolidTorus(2.0, 4*POLES[i][j], 20, 20);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glDisable(GL_LIGHTING);
	if(counter==max_moves)
		drawSolved();
	else
		drawText();
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glColor3f(1,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-80,350,-10,100,-100,100);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}


void mouse(int btn,int mode,int x,int y)
{
	if(btn == 4 && mode == GLUT_DOWN)
	{
		if(counter<max_moves)
		{
			pop(moves[counter][1]);
			push(moves[counter][2],moves[counter][0]);
			counter++;
		}
	}

	glutPostRedisplay();
}

void restart()
{
	int i;
	memset(POLES,0,sizeof(POLES));
	memset(moves,0,sizeof(POLES));
	memset(top,-1,sizeof(top));
	cnt=0,counter=0;
	ycoordinate=0.1;
	max_moves = pow(2,NUM_DISKS)-1;
	for(i=NUM_DISKS;i>0;i--)
	{
		push(0,i);
	}
	tower(NUM_DISKS,0,1,2);
}

void processMenuMain2(int option)
{

}

void processMenuRestart(int option)
{
	if(option==0)
	{
		restart();
		glutPostRedisplay();
	}
}

void processMenuExit(int option)
{
	if(option==0)exit(0);
}

void processMenuSolveCompletely(int option)
{
	int i,j;
	while(counter<max_moves)
	{
		mouse(4,GLUT_DOWN,0,0);
		display();
		for(i=0;i<100000;i++)
			for(j=0;j<100;j++);
	}
}

void createGLUTMenus2()
{
	int menuExit = glutCreateMenu(processMenuExit);
	glutAddMenuEntry("Yes",0);
	glutAddMenuEntry("No",1);
	int menuRestart = glutCreateMenu(processMenuRestart);
	glutAddMenuEntry("Yes",0);
	glutAddMenuEntry("No",1);
	int menuSolveCompletely = glutCreateMenu(processMenuSolveCompletely);
	glutAddMenuEntry("Start",0);
	glutCreateMenu(processMenuMain2);
	glutAddSubMenu("Solve Completely",menuSolveCompletely);
	glutAddSubMenu("Restart",menuRestart);
	glutAddSubMenu("Exit",menuExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuMain1(int option)
{
}

void processMenuNumDisks(int option)
{
	NUM_DISKS=option;
	restart();
	glutPostRedisplay();
}

void createGLUTMenus1()
{
	int menu = glutCreateMenu(processMenuNumDisks);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);
	glutAddMenuEntry("6",6);
	glutAddMenuEntry("7",7);
	glutAddMenuEntry("8",8);
	glutAddMenuEntry("9",9);
	glutAddMenuEntry("10",10);
	int menuExit = glutCreateMenu(processMenuExit);
	glutAddMenuEntry("Yes",0);
	glutAddMenuEntry("No",1);
	glutCreateMenu(processMenuMain1);
	glutAddSubMenu("Number of Disks",menu);
	glutAddSubMenu("Exit",menuExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void strokeString(float x,float y,float sx,float sy,char *string,int width)
{
	char *c;
	glLineWidth(width);
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(sx,sy,0);
	for(c=string; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}

void initfirst()
{
	glClearColor(0.1, 0.1, 0.3, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1000,0,1000,-1,1);
	glMatrixMode(GL_MODELVIEW);
}

void first()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
	strokeString(50,850,0.15,0.15,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING",2);
	strokeString(150,750,0.3,0.3,"SIR M VISVESVARAYA INSTITUTE",4);
	strokeString(300,670,0.3,0.3,"OF TECHNOLOGY",4);
	strokeString(150,500,0.3,0.3,"COMPUTER GRAPHICS MINI PROJECT",2);
	glColor3f(0,1,0);
	strokeString(250,420,0.4,0.4,"TOWER OF HANOI",6);
	glColor3f(1,1,0);
	strokeString(250,220,0.2,0.2,"NUMBER OF DISKS:",3);
	char str[5];
	sprintf(str, "%d", NUM_DISKS);
	strokeString(550,220,0.2,0.2,str,3);
	glColor3f(1,0,0);
	strokeString(50,100,0.17,0.17,"1 . Set the number of disks using the menu",2);
	strokeString(50,50,0.17,0.17,"2 . Press (Enter) to start the simulation",2);
	strokeString(650,200,0.15,0.15,"By:",2);
	glColor3f(0,1,0);
	strokeString(650,160,0.18,0.18,"Ayushi and Harsh Gahlot",2);
	strokeString(650,120,0.18,0.18,"1MV17CS022,1MV17CS038",2);
	strokeString(650,30,0.2,0.2,"Prof. Elaiya Raja",2);
	glColor3f(1,0,0);
	strokeString(650,60,0.15,0.15,"Under the guidance of.",2);
	glutSwapBuffers();
}

void keyboard2(unsigned char c, int x, int y){}
void keyboard(unsigned char c, int x, int y)
{
	switch(c)
	{
		case 13:
			restart();
			init();
			glutDisplayFunc(display);
			createGLUTMenus2();
			glutKeyboardFunc(keyboard2);
			glutMouseFunc(mouse);
		break;
	}
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(1024,720);
	glutInitWindowPosition(100,100);
	glutCreateWindow("tower of hannoi");
	initfirst();
	glutDisplayFunc(first);
	createGLUTMenus1();
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

