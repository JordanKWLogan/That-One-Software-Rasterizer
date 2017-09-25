


#include "Window.h"
#include "Display.h"

#include "Rasterizer.h"

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int  /*nCmdShow*/)
{
	Window wind;
	Display dis; // dis is the bomb

	Rasterizer rast;

	wind.SetHInstance(hInstance);
	wind.Initialize(800, 600);

	dis.CreateDevice(wind.GetHWND(), 800, 600);
	dis.CreateBuffers(200, 100);
	dis.CreateShader();

	wind.ActivateWindow();
	while(1)
	{
		wind.Update();

		dis.StartRender();

		dis.Clear(0,0,0);

		rast.RenderTrinagle({ 2, 2 }, { 40, 24 }, { 2, 50 });

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();
	}
}