


#include "Window.h"
#include "Display.h"

#include "Rasterizer.h"
#include "VertexProcessor.h"

// i c math
#include <cmath>

// time likes to float about
float time;

// should restrict some pointers at some point. they have too much FREEDOM
void derpShader(uint8_t* inData, uint8_t* /*outData*/, Point4D& outPoint)
{
	// some level 20 template magic could help here
	Point4D p = *reinterpret_cast<Point4D*>(inData);
	p.x += sin(time) * 10; // time is a sin. tasty time pi sin
	outPoint = p;
}


int WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int  /*nCmdShow*/)
{
	Window wind;
	Display dis; // dis is the bomb

	Rasterizer rast;
	VertexProcessor vp;

	wind.SetHInstance(hInstance);
	wind.Initialize(800, 600);

	dis.CreateDevice(wind.GetHWND(), 800, 600);
	dis.CreateBuffers(200, 100);
	dis.CreateShader();

	wind.ActivateWindow();

	Point4D points[6] = {
		{2,2,0,0},
		{40,24,0,0},
		{2,50,0,0},

		{80,90,0,0},
		{70, 40,0,0},
		{150,25,0, 0}
	};

	VertexData data = {};
	data.attributeCount = 1;
	data.attributes[0].stride = sizeof(Point4D);
	data.attributes[0].size = sizeof(Point4D);
	data.streams[0] = reinterpret_cast<uint8_t*>(points);

	time = 0;

	// why would you ever want to leave the rasterizer
	while(1)
	{
		wind.Update();

		dis.StartRender();

		dis.Clear(0,0,0);

		vp.ProcessDataSteam(derpShader, 6, data);

		for(int32_t i = 0; i < 2; ++i)
			rast.RenderTrinagle(
				vp.m_LocalVertexPositions[i * 3 + 0],
				vp.m_LocalVertexPositions[i * 3 + 1],
				vp.m_LocalVertexPositions[i * 3 + 2]);

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();

		// super duper precise time measurement
		time += 0.01f;
		// what i said percise not accurate
	}
}