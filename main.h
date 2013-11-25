/*
 * main.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Omneya AbdelFattah
 */

#ifndef MAIN_H_
#define MAIN_H_

GLdouble R; // aspect ration
GLuint buffer;
World* world;  //define the models in the world


int main(int argc, char **argv);
void init() ;
void display(void);
void initAttribute(GLuint program, GLint type, const char* name, int size, GLvoid* offset);
void setWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void setViewport(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void myReshape(GLsizei W, GLsizei H);
void timer(int v);
void mouseMotion(int x, int y);
void mouseClicked(int button, int state, int x, int y);
void keyboardKey(unsigned char key, int x, int y);

#endif /* MAIN_H_ */
