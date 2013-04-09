#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	isDebug = false;

	//load some settings
	settings.loadFile("settings.xml");


	//Set up avg and average bins
	for(int i = 0; i < 512; i++) {
		
		softMagnitude[i]  = 0.0f;

	}

	//Set Up FBO's and Shaders
	debugFbo.allocate(ofGetWidth(),ofGetHeight());
	mainFbo.allocate(ofGetWidth(),ofGetHeight());


	//load song from xml
	ofSoundStreamSetup(2,0,this,48000,512,4);
	string xmlSong = settings.getValue("settings:song", "pipes.wav");
	song.loadSound(xmlSong, true);
	song.play();
	cam.setPosition(0,0,0);
	simpleEmitter.init();
	plinkEmitter.init();
	cube.init();
	cubeField.init();
	glEnable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::update(){

	if(getSub() > 0.58) {
		simpleEmitter.emit();
	}
	if(getPlink() > 0.2 || getHiSynth() > 0.5) {
		plinkEmitter.emit();
	}
	if(getBass() > 0.2) {
		cube.emit();
	}
	cubeField.update(softMagnitude);
	simpleEmitter.update();
	plinkEmitter.update();
	cube.update(getSub());
	cam.setPosition(0,0,cam.getPosition().z - 4);
	cam.setOrientation(ofVec3f(sin((double)ofGetElapsedTimeMillis()/1000)*10,cos((double)ofGetElapsedTimeMillis()/1200)*10,(double)ofGetElapsedTimeMillis()/500));
	cube.position.z = cam.getPosition().z - 2000;
	cubeField.position.z = cam.getPosition().z;
	cubeField.offset += 4;
	simpleEmitter.emitterPosition = ofVec3f(sin((double)ofGetFrameNum()/50)*300,cos((double)ofGetFrameNum()/50)*300,cam.getPosition().z - 3000);
	plinkEmitter.emitterPosition = ofVec3f(sin((double)ofGetFrameNum()/50)*300,cos((double)ofGetFrameNum()/50)*300,cam.getPosition().z - 3000);
	

}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushView();
	mainFbo.begin();
	cam.begin();
	ofClear(0);
	ofBackground(ofColor::fromHsb(127,255,ofMap(getHiSynth(),0,1,-255,255)));
	simpleEmitter.draw();
	plinkEmitter.draw();
	cube.draw();
	cubeField.draw();
	ofPushMatrix();
	ofRotateZ(180);
	cubeField.draw();
	ofPopMatrix();
	cam.end();
	mainFbo.end();
	mainFbo.draw(0,0,ofGetWidth(),ofGetHeight());
	
	ofPopView();

	if(isDebug) {
		drawDebug();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'd') {
		isDebug = !isDebug;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawDebug() {
	debugFbo.begin();
	glDisable(GL_DEPTH_TEST);
	ofPushMatrix();
	ofClear(0);

	ofSetCircleResolution(50);
			
	ofSetColor(200,0,0);
	ofCircle(ofPoint(ofGetWidth() - 350,350),getBass()*350);

	ofSetColor(255,0,200);
	ofCircle(ofPoint(ofGetWidth() - 350,350),getSub()*300);
			
	ofSetColor(0,200,200);
	ofCircle(ofPoint(ofGetWidth() - 350,750),getPlink()*200);

    ofSetColor(0,0,255);
	ofCircle(ofPoint(ofGetWidth() - 350,750),getHiSynth()*200);

		for(int i = 0; i < 512; i++) {
			if(i%2 == 0){
				ofSetColor(255);
			}else{
				ofSetColor(200);
			}
			ofRect(i*20,0,0,20,softMagnitude[i]*100);

			
			
		}

		stringstream bandNumber; 
		bandNumber  << "Band: " << floor((double) mouseX/20);
		ofDrawBitmapString(bandNumber.str(),mouseX,mouseY);
		
	ofPopMatrix();
	
	debugFbo.end();
	debugFbo.draw(0,0,ofGetWidth(),ofGetHeight());
	glEnable(GL_DEPTH_TEST);
}

void testApp::audioOut(float * output, int bufferSize, int nChannels) {
	
	float * fft = ofSoundGetSpectrum(512);
	
	for (int i = 0;i < 512; i++){
		
		// let the smoothed calue sink to zero:
		softMagnitude[i] *= 0.96f;
		

		// take the max, either the smoothed or the incoming:
		if (softMagnitude[i] < fft[i]) softMagnitude[i] = fft[i];
		
	}

}

float testApp::getSub() {
	return ofMap(softMagnitude[1],0,5,0,1);
}

float testApp::getPlink() {
	return ofMap(softMagnitude[17],0,3,0,1);
}

float testApp::getHiSynth() {
	return ofMap(softMagnitude[9],0,0.8,0,1);
}

float testApp::getBass() {
	return ofMap(softMagnitude[6],0,3,0,1);
}

float testApp::getTube() {
	return ofMap(softMagnitude[17],0,3,0,1);
}

