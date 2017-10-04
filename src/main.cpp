#include "ofMain.h"
#include "ofApp.h"

int main(int argc, char *argv[])
{
    ofGLWindowSettings settings;
    settings.width = 960;
    settings.height = 540;
    settings.windowMode = OF_WINDOW;
    settings.setGLVersion(4, 1);
    
    ofCreateWindow(settings);

	ofApp *app = new ofApp();
	app->arguments = vector<string>(argv, argv + argc);
    ofRunApp(app);
}
