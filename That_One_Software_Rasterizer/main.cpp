


#include "Window.h"
#include "Display.h"

#include "Rasterizer.h"
#include "VertexProcessor.h"

// i c math
#include <cmath>

// time likes to float about
float time;

// its nice to have a perspective of life
Point4D perspective[4];

void CreatePerspective(float fov, float aspect, float zn, float zf)
{
	const float yScale = 1 / tan(fov / 2);
	float xScale = yScale * aspect;

	perspective[0] = Point4D{ xScale,      0,                    0, 0 };
	perspective[1] = Point4D{	   0, yScale,                    0, 0 };
	perspective[2] = Point4D{      0,      0,       zf / (zf - zn), -zn * zf / (zf - zn) };
	perspective[3] = Point4D{      0,      0, 1, 0 };
}

float dot(Point4D p0, Point4D p1)
{
	return p0.x * p1.x + p0.y * p1.y + p0.z * p1.z + p0.w * p1.w;
}

// should restrict some pointers at some point. they have too much FREEDOM
void derpShader(uint8_t* inData, uint8_t* /*outData*/, Point4D& outPoint)
{
	// some level 20 template magic could help here
	Point4D p = *reinterpret_cast<Point4D*>(inData);
	p.x += sin(time) * 10; // time is a sin. tasty time pi sin

	Point4D pOut;
	// the best matrix multiplay ever
	pOut.x = dot(perspective[0], p);
	pOut.y = dot(perspective[1], p);
	pOut.z = dot(perspective[2], p);
	pOut.w = dot(perspective[3], p);

	outPoint = pOut;
}


int WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int  /*nCmdShow*/)
{
	Window wind;
	Display dis; // dis is the bomb

	Texture depth;
	TextureDesc desc;
	desc.Width = 200;
	desc.Height = 100;
	desc.Format = DATA_FORMAT::D32;
	depth.CreateTexture(desc);

	Texture color;
	desc.Format = DATA_FORMAT::RGBA8;
	color.CreateTexture(desc);

	Rasterizer rast;
	rast.m_ColorTexture = &color;
	rast.m_DepthTexture = &depth;

	VertexProcessor vp;

	wind.SetHInstance(hInstance);
	wind.Initialize(800, 600);

	dis.CreateDevice(wind.GetHWND(), 800, 600);
	dis.CreateBuffers(200, 100);
	dis.CreateShader();

	wind.ActivateWindow();

	Point4D points[6] = {

		{ 6,   2,  50, 1 },
		{ 16,  5,  50, 1 },
		{ 8,  10,  50, 1 },

		{ 8,  9, 40, 1 },
		{ 7,  4, 45, 1 },
		{ 15, 2, 60, 1 },
	};

	VertexData data = {};
	data.attributeCount = 1;
	data.attributes[0].stride = sizeof(Point4D);
	data.attributes[0].size = sizeof(Point4D);
	data.streams[0] = reinterpret_cast<uint8_t*>(points);

	time = 0;


	CreatePerspective(0.7f, 0.5f, 0.01f, 100);

	// why would you ever want to leave the rasterizer
	while(1)
	{
		wind.Update();

		const uint8_t clearColor[4] = { 0,0,0,0 };
		color.Clear(clearColor);

		const float clearDepth = 1;
		depth.Clear(&clearDepth);

		vp.ProcessDataSteam(derpShader, 6, data);

		rast.m_VERYTEMP_Color[0] = 255;
		rast.m_VERYTEMP_Color[1] = 0;
		rast.m_VERYTEMP_Color[2] = 0;
		rast.m_VERYTEMP_Color[3] = 255;
		rast.RenderTrinagle(
			vp.m_LocalVertexPositions[0 * 3 + 0],
			vp.m_LocalVertexPositions[0 * 3 + 1],
			vp.m_LocalVertexPositions[0 * 3 + 2]);

		rast.m_VERYTEMP_Color[0] = 0;
		rast.m_VERYTEMP_Color[1] = 255;
		rast.m_VERYTEMP_Color[2] = 0;
		rast.m_VERYTEMP_Color[3] = 255;
		rast.RenderTrinagle(
			vp.m_LocalVertexPositions[1 * 3 + 0],
			vp.m_LocalVertexPositions[1 * 3 + 1],
			vp.m_LocalVertexPositions[1 * 3 + 2]);


		dis.StartRender();

		// should really stop hard coding the viewport
		for(int32_t y = 0; y < 100; ++y)
		{
			for(int32_t x = 0; x < 200; ++x)
			{
				uint8_t pixel[4];
				color.ReadPixel(IPoint2D{ x, y }, pixel);
				dis.SetPixel(pixel[0], pixel[1], pixel[1], x, y);
			}
		}

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();

		// super duper precise time measurement
		time += 0.01f;
		// what i said percise not accurate
	}
}