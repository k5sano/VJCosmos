#include "ofMain.h"
#include "ofApp.h"

#ifdef __APPLE__
#import <Foundation/Foundation.h>
#endif

int main(){
#ifdef __APPLE__
	// Disable App Nap so update/draw continues when unfocused (critical for VJ + OSC)
	[[NSProcessInfo processInfo] beginActivityWithOptions:NSActivityUserInitiatedAllowingIdleSystemSleep
	                                              reason:@"VJCosmos must render continuously for Syphon/OSC output"];
#endif
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.setGLVersion(2, 1);  // ofxFluid は GLSL 120 なので GL 2.1
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}

