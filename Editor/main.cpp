#include "Engine.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	ReportMemoryLeaks();

	LOG("Engine Creation ----------------");
	Engine* engine = new Engine;

	int main_return = engine->Loop();

	RELEASE(engine);
	LOG("Bye! :D");

	return main_return;
}