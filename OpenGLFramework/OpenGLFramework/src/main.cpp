#include <OpenGLApplication.h>


int main(int argv, char* argc[])
{
	OpenGlApplication* app = new OpenGlApplication();

	if (app->Start()) {
		while (app->Update())
		{
			app->Draw();
		}
		app->Exit();
	}
	delete app;
	return 0;
}