#include "particlesSimple.h"

void ParticlesSimple::init() {

	sprite.loadImage("feather_sprite.png");
	sprite.resize(30,30);
	emitSize = 2;

	//add a drag
	addForce(ofVec3f(0.999,0.999,0.999));
	addForce(ofVec3f(1.002,1.005,1));

	emitterPosition = ofVec3f(0,0,0);
	for(int i = 0; i < emitSize; i ++) {

		Simple particle; 
		particle.init(sprite,forces, emitterPosition, target);
		particles.push_back(particle);

	}
	emit();
}

void ParticlesSimple::update() {

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

void ParticlesSimple::draw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
	
	for(int i = 0; i < particles.size() ; i++) {

		particles[i].draw();

	}

}

void ParticlesSimple::emit() {

	for(int i = 0; i < emitSize; i ++) {

		Simple particle; 
		particle.init(sprite,forces, emitterPosition, target);
		particles.push_back(particle);

	}

	target.x =0;
	target.y =0;
	target.z = emitterPosition.z - 100;

	frames++;
}

void ParticlesSimple::addForce(ofVec3f force) {

	forces.push_back(force);

}

//Particle Class Definitions

void Simple::init(ofImage & image, vector <ofVec3f> & forcesReference, ofPoint & emitterPosition, ofPoint & targetPosition) {
	sprite = &image;
	velocity = ofVec3f(ofRandom(-5,5), ofRandom(-5,5), ofRandom(-2,2));
	forces = &forcesReference;
	position = emitterPosition;
	target = &targetPosition;
}

void Simple::update() {

	age++;

	for(int i = 0; i < forces->size(); i++) {
		velocity *= forces->at(i);
	}


	position += (ofVec3f(*target) - position)*0.01;

	position += velocity;

}



void Simple::draw() {
	ofEnableAlphaBlending();
	ofSetColor(255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)),255*(ofMap(age,0,500,1,0)));
	ofNoFill();
	ofEllipse(position.x,position.y,position.z,30,30);
	ofFill();
	ofDisableAlphaBlending();
	 
}