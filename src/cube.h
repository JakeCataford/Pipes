#pragma once
#include "ofMain.h"
#include "NoVelParticles.h"

class Cube {

public:
	
	Cube(){};
	~Cube(){};

	void init();
	void update(float influence);
	void draw();
	void emit();

	ofPoint position;

private:

	int fft;
	ofMesh cubeMesh;
	vector <ofPoint> verts;
	vector <NoVelParticles> emitters;
	float age;
	int size;
	int offset;
	


};