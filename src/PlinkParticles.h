#pragma once
#include "ofMain.h"

class PlinkSimple {


public:

	PlinkSimple() :

		age(0)

	{};

	~PlinkSimple() {};

	void init(ofImage & spriteReference, vector <ofVec3f> & forces, ofVec3f & position, ofVec3f & targetPosition);
	void update();
	void draw();
	int age;


private:
	ofVec3f position;
	ofVec3f velocity;
	vector <ofVec3f> * forces;
	ofImage * sprite;
	ofVec3f * target;
	
};


class PlinkParticles {

public:

	PlinkParticles() {};
	~PlinkParticles() {};

	//Core
	void init();
	void emit();
	void update();
	void draw();

	//hooks
	void addForce(ofVec3f forceVector);
	ofVec3f * getForceReference(int index);
	ofPoint emitterPosition;


private:

	vector <ofVec3f> forces;
	vector <PlinkSimple> particles;
	
	ofImage sprite;
	ofVec3f target;
	int emitSize;
	int frames;

};