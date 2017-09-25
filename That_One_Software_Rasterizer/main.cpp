


#include "Window.h"
#include "Display.h"

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int  /*nCmdShow*/)
{
	Window wind;
	Display dis; // dis is the bomb

	wind.SetHInstance(hInstance);
	wind.Initialize(800, 600);

	dis.CreateDevice(wind.GetHWND());
	dis.CreateBuffers(800, 600);
	dis.CreateShader();

	wind.ActivateWindow();
	while(1)
	{
		wind.Update();

		dis.StartRender();

		dis.Clear(126,8,8);

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();
	}
}