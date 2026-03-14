#include "ofMain.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.setGLVersion(2, 1);  // ofxFluid は GLSL 120 なので GL 2.1
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}

