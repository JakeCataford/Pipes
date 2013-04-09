#include "cube.h"

void Cube::init() {
	age = 0;
	offset = 0;
	position = ofVec3f(0,0,0);
	size = 500;
	cubeMesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLE_FAN);
	
	for(int i = 0; i < 8; i++) {
		NoVelParticles e = NoVelParticles();
		e.init();
		emitters.push_back(e);
		
	}

	


}

void Cube::update(float influence) {

	cubeMesh.clear();
	if( ofMap(influence,0,1,0,255) - fft > 100) {
		age += 10;
		offset = ofRandom(0,1500);
	}
	fft = ofMap(influence,0,1,0,255);

	cubeMesh.addVertex(ofVec3f(0,0,0));
	cubeMesh.addVertex(ofVec3f(-size,0,fft));
	cubeMesh.addVertex(ofVec3f(fft,size,0));
	cubeMesh.addVertex(ofVec3f(0,0,size));
	cubeMesh.addVertex(ofVec3f(size,-size,fft));
	cubeMesh.addVertex(ofVec3f(fft,-size + sin(age/100) * 100,-size));
	cubeMesh.addVertex(ofVec3f(size,0,size));
	cubeMesh.addVertex(ofVec3f(-size,size,size));

	vector <ofPoint> verts = cubeMesh.getVertices();

	for(int i = 0; i < emitters.size(); i ++) {
		ofVec3f ut(verts[i]);
		
		ut.rotate(age,ofVec3f(1,0,0));
		ut.rotate(age/2,ofVec3f(0,1,0));
		ut.rotate(age/3,ofVec3f(0,0,1));
		ut += ofVec3f(position.x,position.y,position.z+offset);
		
		emitters[i].emitterPosition = ut;
		emitters[i].update();

	}
	
}


void Cube::draw() {

	ofPushMatrix();
	ofTranslate(position.x,position.y,position.z + offset);
	ofRotateX(age);
	ofRotateY(age/2);
	ofRotateZ(age/3);
	ofSetColor(0,100,200,255);
	cubeMesh.drawFaces();
	ofSetColor(255,255,255,255);
	cubeMesh.drawWireframe();
	age+= 3;
	
	ofPopMatrix();

	for(int i = 0; i < emitters.size(); i ++) {
		ofPushStyle();
		emitters[i].draw();
		ofPopStyle();

	}



}

void Cube::emit() {

	for(int i = 0; i < emitters.size(); i++) {

		emitters[i].emit();

	}

}

