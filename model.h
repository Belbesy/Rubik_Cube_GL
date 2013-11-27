/*
 * model.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Omneya AbdelFattah
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "include/Angel.h"
#include <cstdio>
#include <cmath>
#include <string.h>
#include <vector>

using namespace std;

// ========== constants ==========
const GLfloat SCREENWIDTH = 700; //window size
const GLfloat SCREENHEIGHT = 700;

GLfloat curr_z = 1.0; // NÃºverandi z-hnit
GLfloat theta = 40.0;
GLfloat fovy = 100.0;

bool is_animated = true;

mat4 I(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

//-------------------------------World Class-----------------------------------------
class Model;
class World {
public:

	vector<vec3> points;
	vector<vec4> colors;
	vector<Model*> models;

	GLint model_m;
	GLint view_m;
	GLint projection_m;

	mat4 view, projection;
	GLint program;

	World();
	void addModel(Model* model);
	void addPoint(vec3 point, vec4 color, GLint transf);
	void draw();
	size_t points_count();
	void setProgram(GLint program);

	void update();
	void updateMouseClicked(int button, int state, int x, int y);
	void updateMouseMotion(int x, int y);
};

//-------------------------------Fragment Class--------------------------------------
class Fragment {
public:
	GLenum mode;
	GLint start;
	GLsizei count;
	Fragment(GLenum mode_, GLint start_, GLsizei count_);
	void draw();
};

//-------------------------------Model Class-----------------------------------------
class Model {
public:
	World* world;
	vector<Fragment> fragments;
	mat4 rotation_m;
	mat4 translation_m;
	vec4 position;
	float rotation_angle;
	Model(World* world_);
	void addTringle(vec3 p1, vec3 p2, vec3 p3, vec4 color);
	void addFragment(GLenum type, GLsizei count);
	void rotate(float theta, int d);
	virtual void draw(World* world);
	virtual void update();
};

//-------------------------------Cube Class------------------------------------------

float x = 0.5;

// Vertices of a unit cube centered at origin, sides aligned with axes
vec3 vertices[8] = { vec3(-x, -x, x), vec3(-x, x, x), vec3(x, x, x), vec3(x, -x,
		x), vec3(-x, -x, -x), vec3(-x, x, -x), vec3(x, x, -x), vec3(x, -x, -x) };

// RGBA colors
vec4 vertex_colors[8] = { vec4(0.0, 0.0, 0.0, 1.0), // black
vec4(1.0, 0.0, 0.0, 1.0), // red
vec4(1.0, 1.0, 0.0, 1.0), // yellow
vec4(0.0, 1.0, 0.0, 1.0), // green
vec4(0.0, 0.0, 1.0, 1.0), // blue
vec4(1.0, 0.0, 1.0, 1.0), // magenta
vec4(1.0, 1.0, 1.0, 1.0), // white
vec4(0.0, 1.0, 1.0, 1.0) // cyan
		};

class Cube: public Model {
public:

	int up_color = 1;     // white
	int front_color = 3;  //red

	Cube(World* world, vec4 loc);
	void quad(int a, int b, int c, int d);
	int get_back_color();
	int get_down_color();
	int get_left_color();
	int get_right_color();
};

//--------------------------------- Rubik's cube class ------------------------------

class Rubik: public Model {
	Cube* cubes[3][3][3];
public:
	Rubik(World* world);
	virtual void draw();
	void update();
	bool is_solved();
	void animation(int axis, int order, bool is_clockwise);
	void update_color(int axis, int order, bool is_clockwise);
	void print();
	void init(int numofRotation);
};

void Model::update() {

}

//-------------------------------World Implementation--------------------------------
void World::setProgram(GLint program) {

	this->program = program;
	this->model_m = glGetUniformLocation(program, "Model");
	this->view_m = glGetUniformLocation(program, "View");
	this->projection_m = glGetUniformLocation(program, "Projection");

	vec4 eye(4, 3, 3, 0.0);
	vec4 at(0.0, 0.0, 0.0, 0.0);
	vec4 up(0.0, 1.0, 0.0, 0.0);

	// Set the model view matrix
	view = LookAt(eye, at, up);
	view *= RotateY(theta);

	// glUniformMatrix4fv( modelview, 1, GL_TRUE, mv );

	// Set the projection
	projection = Perspective(fovy, 1.0, 0.2, 100.0);

	glUniformMatrix4fv(this->model_m, 1, GL_TRUE, I);
	glUniformMatrix4fv(this->view_m, 1, GL_TRUE, view);
	glUniformMatrix4fv(this->projection_m, 1, GL_TRUE, projection);
}

void World::addModel(Model* model) {
	models.push_back(model);
}

void World::addPoint(vec3 point, vec4 color, GLint transform) {
	points.push_back(point);
	colors.push_back(color);
}

void World::draw() {
	for (size_t i = 0; i < models.size(); i++)
		models[i]->draw(this);
}

size_t World::points_count() {
	return points.size();
}

World::World() {
	addModel(new Rubik(this));
}

void World::updateMouseClicked(int button, int state, int x, int y) {
}

void World::updateMouseMotion(int x, int y) {

}

void World::update() {
	for (size_t i = 0; i < models.size(); i++) {
		models[i]->update();
	}
}

//----------------------------------Fragment Implementation-----------------------------

Fragment::Fragment(GLenum mode_, GLint start_, GLsizei count_) {
	this->mode = mode_;
	this->start = start_;
	this->count = count_;
}

void Fragment::draw() {
	glDrawArrays(mode, start, count);
}

//----------------------------------Model Implementation--------------------------------

Model::Model(World* world_) {
	this->world = world_;
	//this->matrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

//void Model::addTringle(vec3 p1, vec3 p2, vec3 p3 , vec4 color) {
//	addFragment(GL_TRIANGLES, 3);
//	world->addPoint(p1, color, world->models.size());
//	world->addPoint(p2, color, world->models.size());
//	world->addPoint(p3, color, world->models.size());
//}

void Model::addFragment(GLenum type, GLsizei count) {
	fragments.push_back(Fragment(type, world->points_count(), count));
}

void Model::draw(World* world) {

	mat4 model = rotation_m * translation_m;

	glUniformMatrix4fv(world->model_m, 1, GL_TRUE, model);

	for (size_t i = 0; i < fragments.size(); i++)
		fragments[i].draw();

}

void Model::rotate(float theta, int d) {

	rotation_angle += theta;

	switch (d) {
	case 0:
		this->rotation_m = RotateX(rotation_angle);
		break;
	case 1:
		this->rotation_m = RotateY(rotation_angle);
		break;
	case 2:
		this->rotation_m = RotateZ(rotation_angle);
		break;
	default:
		break;
	}

	double val = (int) rotation_angle % 90;
	if (val == 0)
		is_animated = false;
	else
		is_animated = true;
}
//----------------------------Cube Implementation---------------------------------------

void Cube::quad(int a, int b, int c, int d) {
	world->addPoint(vertices[a], vertex_colors[a], world->models.size());
	world->addPoint(vertices[b], vertex_colors[a], world->models.size());
	world->addPoint(vertices[c], vertex_colors[a], world->models.size());
	world->addPoint(vertices[a], vertex_colors[a], world->models.size());
	world->addPoint(vertices[c], vertex_colors[a], world->models.size());
	world->addPoint(vertices[d], vertex_colors[a], world->models.size());
}

Cube::Cube(World* world, vec4 loc) :
		Model(world) {

	this->position = loc;
	mat4 m1(  //
			1, 0, 0, loc.x, 0, 1, 0, loc.y, 0, 0, 1, loc.z, 0, 0, 0, 1);

	this->translation_m = m1;

	//A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles,36 points
	// triangle 1
	addFragment(GL_TRIANGLES, 36);
	quad(5, 4, 0, 1);
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(3, 0, 4, 7);
}

int Cube::get_down_color()
{
	return - up_color;
}

int Cube::get_back_color()
{
	return - front_color;
}

int Cube::get_left_color()
{
	if ((up_color>0 && front_color >0)|| (up_color>0 && front_color>0))
		return 2;
	else if((up_color>0 && front_color<0) || (up_color<0 && front_color>0))
		return -2;
}

int Cube::get_right_color()
{
	return - get_left_color();
}

//==========================================================================

Rubik::Rubik(World* world) :
		Model(world) {

	memset(cubes, 0, sizeof cubes);
	int delta[3] = { -1, 0, 1 };
	float CUBE_SIDE = 1.0;
	float GAP = 0.03;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {

				Cube* c = new Cube(world,
						vec4(delta[i] * (CUBE_SIDE + GAP),
								delta[j] * (CUBE_SIDE + GAP),
								delta[k] * (CUBE_SIDE + GAP), 0));
				printf("New Cube Centered @(%2.2f, %2.2f, %2.2f)\n",
						c->position.x, c->position.y, c->position.z);
				world->addModel(c);
				cubes[i][j][k] = c;

			}
		}
	}

	init(3);

}

void Rubik::init(int numofRoatation)
{
	for(int i=0;i<numofRoatation;i++)
	{
		int axis = rand() % 3;
		int order =(rand()+rand())%3;
		int direction =rand()%2;
		for(int j=0;j<90;j++)
		{
			//for debug
			cout << "axis : " <<axis<< endl;
			cout << "order : " <<order<< endl;
			cout << "direction :  " << (bool)direction << endl;
			
			
			animation(axis,order,(bool)direction);
			update_color(axis,order,direction);
			//handel matrix positions
			cout << "---------" << endl;
		}
		cout << "-----------------------------------------" << endl;
	}
}

void Rubik::draw() {
	//	for(size_t i=0; i<cubes.size(); )
}

//clock_t last_time =0;
/* if solved --> return
 * else if animation don't do any thing
 * change color faces in each of them
 * else rearrange cubes in the matrix
 * check solved if all has same 2 faces case
 */

int w= 0;
void Rubik::print()
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			for(int k=0;k<3;k++)
			{
				cout << cubes[i][j][k]->front_color << "   ";
			}
			cout << endl;
		}
	cout << "=====================================" << endl;
}

void Rubik::update() {
	Model::update();

//	if (is_solved())
//		return;
//	if (w==0)
//	   print();
//
//	if(w == 1)
//	{
//		print();
//		return;
//	}

//	int axis = 1, order = 1;
//	if (is_animated)
//		animation(axis, order, true);
//	else
//	{
//		update_color(axis, order, true);
//		//change positon int matrix
//		//is_animated = true;
//
//		w++;
//	}

}

void Rubik::update_color(int axis, int order, bool is_clockwise) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) {
				switch (axis) {
				case 0:
					i = 3;
					if (is_clockwise) {
						int down =  cubes[order][j][k]->get_down_color();
						 cubes[order][j][k]->up_color =  cubes[order][j][k]->front_color;
						 cubes[order][j][k]->front_color = down;
					} else {
						int back =  cubes[order][j][k]->get_back_color();
						 cubes[order][j][k]->front_color =  cubes[order][j][k]->up_color;
						 cubes[order][j][k]->up_color = back;
					}
					break;
				case 1:
					j = 3;
					if (is_clockwise) {
						cubes[i][order][k]->front_color = cubes[i][order][k]->get_right_color();
					} else {
						cubes[i][order][k]->front_color = cubes[i][order][k]->get_left_color();
					}
					break;
				case 2:
					k = 3;
					if (is_clockwise) {
						cubes[i][j][order]->up_color = cubes[i][j][order]->get_left_color();
					} else {
						cubes[i][j][order]->up_color = cubes[i][j][order]->get_right_color();
					}
					break;
				default:
					break;
				}
			}
}

void Rubik::animation(int axis, int order, bool is_clockwise) {
	int theta;
	if (is_clockwise)
		theta = -1;
	else
		theta = 1;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) {
				switch (axis) {
				case 0:
					i = 3;
					cubes[order][j][k]->rotate(theta, axis);
					break;
				case 1:
					j = 3;
					cubes[i][order][k]->rotate(theta, axis);
					break;
				case 2:
					k = 3;
					cubes[i][j][order]->rotate(theta, axis);
					break;
				default:
					break;
				}
			}
}

bool Rubik::is_solved() {
	int current_up_color = cubes[0][0][0]->up_color;
	int current_front_color = cubes[0][0][0]->front_color;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; k++)
				if (current_up_color != cubes[i][j][k]->up_color
						|| current_front_color != cubes[i][j][k]->front_color)
					return false;
	return true;

}
#endif /* MODEL_H_ */
