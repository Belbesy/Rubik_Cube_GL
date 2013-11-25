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

GLfloat curr_z = 1.0; // Núverandi z-hnit
GLfloat theta = 40.0;
GLfloat fovy = 100.0;

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
vec3 vertices[8] = { vec3(-x, -x, x), vec3(-x, x, x), vec3(x, x, x), vec3(x, -x, x), vec3(-x, -x, -x), vec3(-x, x, -x), vec3(x, x, -x), vec3(x, -x, -x) };

// RGBA olors
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

	Cube(World* world, vec4 loc);
	void quad(int a, int b, int c, int d);
};

//--------------------------------- Rubik's cube class ------------------------------

class Rubik: public Model {
	Cube* cubes[3][3][3];
public:
	Rubik(World* world);
	virtual void draw();
	void update();
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

void pm(mat4 m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << " " << m[i][j];
		}
		cout << endl;
	}

	cout << "==============================" << endl;
}

Cube::Cube(World* world, vec4 loc) :
	Model(world) {

	this->position = loc;
	mat4 m1(//
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

				Cube* c = new Cube(world, vec4(delta[i] * (CUBE_SIDE + GAP), delta[j] * (CUBE_SIDE + GAP), delta[k] * (CUBE_SIDE + GAP), 0));
				printf("New Cube Centered @(%2.2f, %2.2f, %2.2f)\n", c->position.x, c->position.y, c->position.z);
				world->addModel(c);
				cubes[i][j][k] = c;

			}
		}
	}

	/*
	 *
	 *
	 if(k==2){
	 c->rotate(90, 2);
	 }
	 */
}

void Rubik::draw() {
	//	for(size_t i=0; i<cubes.size(); )
}

//clock_t last_time =0;
void Rubik::update() {
	Model::update();

	int i, j, k=0;

	for (i = 0; i < 3; i++)
		for ( j = 0; j < 3; j++)
			//for (int k = 0; k < 3; k++)
			{
				cubes[i][j][k]->rotate(1, 2);
			}
}

#endif /* MODEL_H_ */
