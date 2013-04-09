#include "PlinkParticles.h"

void PlinkParticles::init() {

	sprite.loadImage("feather_sprite.png");
	sprite.resize(30,30);
	emitSize = 1;

	addForce(ofVec3f(1.02,1.005,1));

	emitterPosition = ofVec3f(0,0,0);
	for(int i = 0; i < emitSize; i ++) {

		PlinkSimple particle; 
		particle.init(sprite,forces, emitterPosition, target);
		particles.push_back(particle);

	}
	emit();
}

void PlinkParticles::update() {

	int oldParticles = 0;;


	for(int i = 0; i < particles.size(); i++) {

		particles[i].update();
		if(particles[i].age > 700) {
			oldParticles++;
		}
		
	}

	for(int i = 0; i < oldParticles; i++) {

		particles.erase(particles.begin(),particles.begin()+1);

	}


}

void PlinkParticles::draw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
	
	for(int i = 0; i < particles.size() ; i++) {

		particles[i].draw();

	}

}

void PlinkParticles::emit() {

	for(int i = 0; i < emitSize; i ++) {

		PlinkSimple particle; 
		particle.init(sprite,forces, ofVec3f(ofRandom(-1000,1000),ofRandom(-1000,1000),emitterPosition.z), target);
		particles.push_back(particle);

	}

	target.x =0;
	target.y =0;
	target.z = emitterPosition.z - 100;

	frames++;
}

void PlinkParticles::addForce(ofVec3f force) {

	forces.push_back(force);

}

//Particle Class Definitions

void PlinkSimple::init(ofImage & image, vector <ofVec3f> & forcesReference, ofPoint & emitterPosition, ofPoint & targetPosition) {
	sprite = &image;
	velocity = ofVec3f(ofRandom(-5,5), ofRandom(-5,5), ofRandom(-2,2));
	forces = &forcesReference;
	position = emitterPosition;
	target = &targetPosition;
}

void PlinkSimple::update() {

	age++;

	for(int i = 0; i < forces->size(); i++) {
		velocity *= forces->at(i);
	}


	position += (ofVec3f(*target) - position)*0.01;

	position += velocity;

}



void PlinkSimple::draw() {
	ofEnableAlphaBlending();
	ofSetColor(0*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)));
	ofEllipse(position.x,position.y,position.z,30,30);
	ofDisableAlphaBlending();
	 
}