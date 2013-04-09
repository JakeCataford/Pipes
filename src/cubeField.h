#pragma once
#include "ofMain.h"

class CubeField {

public:
	
	CubeField(){};
	~CubeField(){};

	void init();
	void update(float * arrayOfBands);
	void draw();

	ofPoint position;
	int offset;

private:

	float * arrayOfFftBands;
	ofMesh cubeMesh;
	vector <ofPoint> verts;
	
	int age;
	


};