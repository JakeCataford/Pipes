#include "NoVelParticles.h"

void NoVelParticles::init() {

	sprite.loadImage("feather_sprite.png");
	sprite.resize(30,30);
	emitSize = 1;
	emitterPosition = ofVec3f(0,0,0);
	for(int i = 0; i < emitSize; i ++) {

		VelSimple particle; 
		particle.init(sprite,forces, emitterPosition, target);
		particles.push_back(particle);

	}
	emit();
}

void NoVelParticles::update() {

	int oldParticles = 0;;


	for(int i = 0; i < particles.size(); i++) {

		particles[i].update();
		if(particles[i].age > 200) {
			oldParticles++;
		}
		
	}

	for(int i = 0; i < oldParticles; i++) {

		particles.erase(particles.begin(),particles.begin()+1);

	}


}

void NoVelParticles::draw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
	
	for(int i = 0; i < particles.size() ; i++) {

		particles[i].draw();

	}

}

void NoVelParticles::emit() {

	for(int i = 0; i < emitSize; i ++) {

		VelSimple particle; 
		particle.init(sprite,forces, emitterPosition, target);
		particles.push_back(particle);

	}

	target.x =0;
	target.y =0;
	target.z = emitterPosition.z - 100;

	frames++;
}

void NoVelParticles::addForce(ofVec3f force) {

	forces.push_back(force);

}

//Particle Class Definitions

void VelSimple::init(ofImage & image, vector <ofVec3f> & forcesReference, ofPoint & emitterPosition, ofPoint & targetPosition) {
	sprite = &image;
	
	forces = &forcesReference;
	position = emitterPosition;
	target = &targetPosition;
}

void VelSimple::update() {

	age++;

}



void VelSimple::draw() {
	ofEnableAlphaBlending();
	ofSetColor(0*(ofMap(age,0,500,1,0)),100*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)));
	ofEllipse(position.x,position.y,position.z,5,5);
	ofDisableAlphaBlending();
	 
}