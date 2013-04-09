#pragma once

#include "ofMain.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Sample.h"
#include "fft.h"
#include "particlesSimple.h"
#include "cube.h"
#include "cubeField.h"
#include "PlinkParticles.h"

#define BUFFER_SIZE 512
#define NUM_WINDOWS 80
#define NUM_OF_AVG 25

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void audioOut(float * output, int bufferSize, int nChannels);
		void drawDebug();
		float getAvgBin(int bin);
		float getSub();
		float getPlink();
		float getHiSynth();
		float getBass();
		float getTube();

	private:
		static const int SAMPLE_RATE = 44100;
		static const int LEFT = 1;
		static const int RIGHT = 2;
		static const int MIX = 0;

		float softMagnitude[BUFFER_SIZE];
		float rawAmp;
		float* audioInput;

		float softAmplitude;
		float subs;

		CubeField cubeField;

		ofFbo debugFbo;
		ofFbo mainFbo;
		ofSoundPlayer song;
		ofxXmlSettings settings;

		ParticlesSimple simpleEmitter;
		PlinkParticles plinkEmitter;
		ofCamera cam;
		Cube cube;
		bool isDebug;
		
};
