#include "Sample.h"
#include "ofMain.h"

Sample::~Sample()
{
    //delete myPath;
    delete myData;
    myChunkSize = NULL;
    mySubChunk1Size = NULL;
    myFormat = NULL;
    myChannels = NULL;
    mySampleRate = NULL;
    myByteRate = NULL;
    myBlockAlign = NULL;
    myBitsPerSample = NULL;
    myDataSize = NULL;
}

// empty constructor
Sample::Sample()
{
    position = 0;
    isLooping = false;
}

// constructor takes a wav path
Sample::Sample(string tmpPath)
{
    position = 0;
    isLooping = false;

    myPath = tmpPath;
    read();
}

void Sample::setPath(string tmpPath)
{
    myPath = tmpPath;

}

void Sample::setLooping(bool loop)
{
    isLooping = loop;
}

void Sample::load(string tmpPath) {
    myPath = tmpPath;
	read();
}

void Sample::generateWaveForm(vector<MiniMaxima> * _waveForm) {

	_waveForm->clear();
	
	bool loopState = isLooping;
	setLooping(false);
	
	// waveform display method based on this discussion http://answers.google.com/answers/threadview/id/66003.html
	
	while ((long)position<getLength()) {
		
		MiniMaxima mm;
		mm.minL = mm.minR = mm.maxL = mm.maxR = 0;
		
		for (int i = 0; i < 256; i++){
			
			float sampleL = play()*0.5;
			float sampleR = play()*0.5;
			
			mm.minL = MIN(mm.minL, sampleL);
			mm.minR = MIN(mm.minR, sampleR);
			mm.maxL = MAX(mm.maxL, sampleL);
			mm.maxR = MAX(mm.maxR, sampleR);
			
		}
		
		_waveForm->push_back(mm);
		
 		//cout << mm.minR << " :: " << mm.maxR << " :: " << mm.minL << " :: " << mm.maxL << endl;
	}
	
	position = 0;
	setLooping(loopState);
}

void Sample::drawWaveForm(int _x, int _y, int _w, int _h, vector<MiniMaxima> * _waveForm) 
{
	
	float waveFormZoomX = (float)_waveForm->size()/(float)_w;
	
	glPushMatrix();
	
	glTranslated(_x, _y, 0);
	
	for (int i = 0; i < _waveForm->size(); i++){
		ofSetColor(255, 0, 0);
		ofLine((i-1)/waveFormZoomX, _waveForm->at(i).minR*(float)_h, i/waveFormZoomX, _waveForm->at(i).maxR*(float)_h);
		ofSetColor(0, 0, 255);
		ofLine((i-1)/waveFormZoomX, (float)_h+_waveForm->at(i).minL*(float)_h, i/waveFormZoomX, (float)_h+_waveForm->at(i).maxL*(float)_h);
	}
	
	ofSetColor(0, 255, 0);
	
	float waveFormDisplayScale = getLength()/_w;
	
	ofLine(position/waveFormDisplayScale, -(float)_h*0.5, position/waveFormDisplayScale, (float)_h*1.5);
	
	glPopMatrix();
}

double Sample::play() {
	long length=myDataSize*0.5;
	double remainder;
	short* buffer = (short *)myData;
	position=(position+1);
	remainder = position - (long) position;
	if ((long) position>length) {
	    if(isLooping) {
            position=0;
	    }
        else {
            return 0;
	    }
	}

//	output = (double) (buffer[1+(long)position])/32767;//no interpolation
	output = (double) ((1-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767;//linear interpolation
	return(output);
}

double Sample::play(double speed) {
	double remainder;
	double x,y,z;
	long length=myDataSize*0.5;
	short* buffer = (short *)myData;
	position=(position+speed);

	if ((long) position>length) {
	    if(isLooping) {
            position=0;
	    }
        else {
            return 0;
	    }
	}

	remainder = position - (long) position;
	x=(3*((buffer[1+(long)position]-buffer[2+(long)position])-(buffer[(long)position]-buffer[3+(long)position])))*0.5;
	y=(2*(buffer[2 + (long)position]+buffer[(long)position]))-((5*(buffer[1+(long) position] + buffer[3+(long) position]))*0.5);
	z=(buffer[2 + (long)position]+buffer[(long)position])*0.5;
//	output= (double) (x*pow(remainder,3) + y*pow(remainder,2) + ((z*remainder) + buffer[1+(long) position]))/32767;
//	output = (double) (buffer[1+(long)position])/32767;//no interpolation
	output = (double) ((1-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767;//linear interpolation
	return(output);
}

//double Sample::play(double frequency, double start, double end) {
//	double remainder;
//	short* buffer = (short *)myData;
//	if (position<start) {
//		position=start;
//	}
//	if ( position >= end ) position = start;
//	position += ((end-start)/(playbackSamplerRate/(frequency*2)));
//	remainder = position - (long) position;
////	output = (double) (buffer[1+(long)position])/32767;//no interpolation
//	output = (double) ((1-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767;//linear interpolation
//	return(output);
//}



//double Sample::stretch(double pitch, double frequency) {
//	short* buffer = (short *)myData;
//
//
//
//}


long Sample::getLength()
{
	long length;
	length=myDataSize*0.5;
	return(length);
}
double Sample::getPosition()
{
	return position;
}
void Sample::setPosition(double _position)
{
	position = _position;
}
// return a printable summary of the wav file
char* Sample::getSummary()
{
    char *summary = new char[250];
    sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
    std::cout << myDataSize;
    return summary;
}

int Sample::getChannels()
{
    return myChannels;
}

// write out the wav file
bool Sample::save()
{
    ofToDataPath(myPath);
    fstream myFile (myPath.c_str(), ios::out | ios::binary);

    // write the wav file per the wav file format
    myFile.seekp (0, ios::beg);
    myFile.write ("RIFF", 4);
    myFile.write ((char*) &myChunkSize, 4);
    myFile.write ("WAVE", 4);
    myFile.write ("fmt ", 4);
    myFile.write ((char*) &mySubChunk1Size, 4);
    myFile.write ((char*) &myFormat, 2);
    myFile.write ((char*) &myChannels, 2);
    myFile.write ((char*) &mySampleRate, 4);
    myFile.write ((char*) &myByteRate, 4);
    myFile.write ((char*) &myBlockAlign, 2);
    myFile.write ((char*) &myBitsPerSample, 2);
    myFile.write ("data", 4);
    myFile.write ((char*) &myDataSize, 4);
    myFile.write (myData, myDataSize);

    return true;
}



// read a wav file into this class
bool Sample::read()
{
    myPath = ofToDataPath(myPath,true).c_str();
	cout << myPath << endl;
    ifstream inFile( myPath.c_str(), ios::in | ios::binary);

    //printf("Reading wav file...\n"); // for debugging only

    inFile.seekg(4, ios::beg);
    inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize

    inFile.seekg(16, ios::beg);
    inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size

    //inFile.seekg(20, ios::beg);
    inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM

    //inFile.seekg(22, ios::beg);
    inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)

    //inFile.seekg(24, ios::beg);
    inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the Samplerate

    //inFile.seekg(28, ios::beg);
    inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate

    //inFile.seekg(32, ios::beg);
    inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign

    //inFile.seekg(34, ios::beg);
    inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitsperSample
	
    inFile.seekg(40, ios::beg);
    inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data

    // read the data chunk
    myData = new char[myDataSize];
    inFile.seekg(44, ios::beg);
    inFile.read(myData, myDataSize);

    inFile.close(); // close the input file

    return true; // this should probably be something more descriptive
}
