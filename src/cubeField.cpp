#include "cubeField.h"

void CubeField::init() {

	position = ofVec3f(0,0,0);
	offset = 0;

	cubeMesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLE_STRIP);

}

void CubeField::update(float * influence) {

	arrayOfFftBands = influence;
	
}


void CubeField::draw() {
	cubeMesh.clear();
	if(offset >= 100) {
		offset = 0;
	}
	for(int i = 0 ; i < 30; i++) {
		for(int j = 0; j < 200; j++) {
			ofPushMatrix();
			cubeMesh.addVertex(ofVec3f(-1000 + j*10,-200 + arrayOfFftBands[j % 30] * 50,position.z + offset  - 100*i));
			cubeMesh.addColor(ofColor::fromHsb((i/30) * 255,  arrayOfFftBands[i % 30]*255, 255 - i));
			ofPopMatrix();
		}
	}
	cubeMesh.drawVertices();
	age++;


}

