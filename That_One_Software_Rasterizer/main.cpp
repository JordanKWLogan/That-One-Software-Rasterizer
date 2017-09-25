


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

	dis.CreateDevice(wind.GetHWND());
	dis.CreateBuffers(800, 600);
	dis.CreateShader();

	wind.ActivateWindow();
	while(1)
	{
		wind.Update();

		dis.StartRender();

		dis.Clear(0,0,0);

		rast.RenderTrinagle({ 10, 10 }, { 10, 500 }, { 400, 240 });

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();
	}
}