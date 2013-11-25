#include "include/Angel.h"
#include "model.h"
#include <math.h>

using namespace std;

World* world;
GLuint buffer;
GLdouble R;



void initAttribute(GLuint program, GLint type, const char* name, int size, GLvoid* offset) {
	GLuint loc = glGetAttribLocation(program, name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, type, GL_FALSE, 0, offset);
}
void setWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)

{

	glMatrixMode(GL_PROJECTION); //specify which matrix is the current matrix

	glLoadIdentity(); //  replace the current matrix with the identity matrix

	gluOrtho2D(left, right, bottom, top); //define a 2D orthographic projection matrix

	//reset aspect ratio

	R = fabs((right - left) / (top - bottom));

}

void setViewport(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) {
	glViewport((int) left, (int) right, (int) (right - left), (int) (bottom - top)); //determins the portion of the window to which OpenGL is drawing to.
}

void init() {
	setWindow(0, SCREENWIDTH, 0, SCREENHEIGHT);
	setViewport(0, SCREENWIDTH, 0, SCREENHEIGHT);
	//memset(board, 0, sizeof(board));


	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	int c_sz = world->colors.size() * sizeof(vec4);
#define v_sz  (world->points.size() * sizeof(vec3))

	glBufferData(GL_ARRAY_BUFFER, c_sz + v_sz , NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0          , v_sz, &(world->points[0]));
	glBufferSubData(GL_ARRAY_BUFFER, v_sz       , c_sz, &(world->colors[0]));

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);
	world->setProgram(program);
	// Initialize the vertex position attribute from the vertex shader
	initAttribute(program ,GL_FLOAT, string("vPosition").c_str() , 3, BUFFER_OFFSET(0));
	initAttribute(program ,GL_FLOAT, string("vColor").c_str()    , 4, BUFFER_OFFSET(v_sz));
}


void myReshape(GLsizei W, GLsizei H) {
	if (R > W / H) //use global window aspect ratio
		setViewport(0, W, 0, W / R);
	else
		setViewport(0, H * R, 0, H);

}
void display(void) {
	glClearColor(.90, .90, .90, 1.0); // set background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	world->update();
	 world->draw();
	glutSwapBuffers();
}

void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}
void mouseMotion(int x, int y) {

}

void mouseClicked(int button, int state, int x, int y){

}

void keyboardKey(unsigned char key, int x, int y) {

}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glShadeModel(GL_SMOOTH);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	glutCreateWindow("Rubik's Cube");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	glewInit();
	World game;
	world = &game;
	init();

	glutReshapeFunc(myReshape);
	glutMouseFunc(mouseClicked);
	glutPassiveMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboardKey);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);

	glutMainLoop();
	return 0;
}


