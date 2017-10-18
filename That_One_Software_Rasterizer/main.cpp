


#include "Window.h"
#include "Display.h"

#include "Rasterizer.h"
#include "VertexProcessor.h"

#include "SceneImporter.h"

// i c math
#include <cmath>

static int32_t SIZE_X = 200;
static int32_t SIZE_Y = 100;

// time likes to float about
float time;

// its nice to have a perspective of life
Point4D perspective[4];

// NOONE LOOK AT THIS
Point4D rotate[4];

void CreatePerspective(float fov, float aspect, float zn, float zf)
{
	const float yScale = 1 / tan(fov / 2);
	float xScale = yScale * aspect;

	perspective[0] = Point4D{ xScale,      0,                    0, 0 };
	perspective[1] = Point4D{	   0, yScale,                    0, 0 };
	perspective[2] = Point4D{      0,      0,       zf / (zf - zn), -zn * zf / (zf - zn) };
	perspective[3] = Point4D{      0,      0, 1, 0 };
}

void CreateCamera(float x)
{
	const float cosX = cos(x);
	const float sinX = sin(x);

#if 0
	rotate[0] = Point4D{  cosX, -sinX, 0, 0 };
	rotate[1] = Point4D{  sinX,  cosX, 0, 0 };
	rotate[2] = Point4D{     0,     0, 1, 0 };
	rotate[3] = Point4D{     0,     0, 0, 1 };
#endif

#if 1
	rotate[0] = Point4D{  cosX,  0,  sinX, 0 };
	rotate[1] = Point4D{     0,  1,     0, -150 }; // move the world down.
	rotate[2] = Point4D{ -sinX,  0,  cosX, 0 };
	rotate[3] = Point4D{  0,     0,     0, 1 };
#endif

#if 0
	rotate[0] = Point4D{ 1,    0,     0, 0 };
	rotate[1] = Point4D{ 0, cosX, -sinX, 0 };
	rotate[2] = Point4D{ 0, sinX,  cosX, 0 };
	rotate[3] = Point4D{ 0,    0,     0, 1 };
#endif
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
	//p.x += sin(time) * 10; // time is a sin. tasty time pi sin

	Point4D pRot;
	pRot.x = dot(rotate[0], p);
	pRot.y = dot(rotate[1], p);
	pRot.z = dot(rotate[2], p);
	pRot.w = dot(rotate[3], p);

	Point4D pOut;
	// the best matrix multiplay ever
	pOut.x = dot(perspective[0], pRot);
	pOut.y = dot(perspective[1], pRot);
	pOut.z = dot(perspective[2], pRot);
	pOut.w = dot(perspective[3], pRot);

	pOut.y = -pOut.y;

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
	desc.Width = SIZE_X;
	desc.Height = SIZE_Y;
	desc.Format = DATA_FORMAT::D32;
	depth.CreateTexture(desc);

	Texture color;
	desc.Format = DATA_FORMAT::RGBA8;
	color.CreateTexture(desc);

	Rasterizer rast;
	rast.m_ColorTexture = &color;
	rast.m_DepthTexture = &depth;

	rast.SetViewPort(IPoint2D{ SIZE_X, SIZE_Y });

	VertexProcessor vp;

	wind.SetHInstance(hInstance);
	wind.Initialize(800, 600);

	dis.CreateDevice(wind.GetHWND(), 800, 600);
	dis.CreateBuffers(SIZE_X, SIZE_Y);
	dis.CreateShader();

	wind.ActivateWindow();


	std::vector<Vertex>	verts;
	std::vector<uint32_t> indices;
	ImportScene("Sponza\\sponza.obj", verts, indices);


	time = 0;


	CreatePerspective(1.0f, float(SIZE_Y) / SIZE_X, 0.1f, 1000);

	// why would you ever want to leave the rasterizer
	while(1)
	{
		wind.Update();

		CreateCamera(time);

		const uint8_t clearColor[4] = { 0,0,0,0 };
		color.Clear(clearColor);

		const float clearDepth = 1;
		depth.Clear(&clearDepth);


		for(uint32_t i = 0; i < indices.size() / 3; i += 3)
		{
			// uuuh i think i need to add indices to the vertex processer or create a primitive assamblier

			Point4D temp[3];
			temp[0] = verts[indices[i + 0]].pos;
			temp[1] = verts[indices[i + 1]].pos;
			temp[2] = verts[indices[i + 2]].pos;

			VertexData data = {};
			data.attributeCount = 1;
			data.attributes[0].stride = sizeof(Point4D);
			data.attributes[0].size = sizeof(Point4D);
			data.streams[0] = reinterpret_cast<uint8_t*>(temp);

			vp.ProcessDataSteam(derpShader, 3, 0, data);

			rast.m_VERYTEMP_Color[0] = 255;
			rast.m_VERYTEMP_Color[1] = 0;
			rast.m_VERYTEMP_Color[2] = 0;
			rast.m_VERYTEMP_Color[3] = 255;
			rast.RenderTrinagle(
				vp.m_LocalVertexPositions[0 * 3 + 0],
				vp.m_LocalVertexPositions[0 * 3 + 1],
				vp.m_LocalVertexPositions[0 * 3 + 2]);
		}


		dis.StartRender();


		// find depth range
		float depthMin = 1;
		float depthMax = 0;
		for(int32_t y = 0; y < SIZE_Y; ++y)
		{
			for(int32_t x = 0; x < SIZE_X; ++x)
			{
				float depthValue;
				depth.ReadPixel(IPoint2D{ x, y }, &depthValue);

				depthMin = depthMin < depthValue ? depthMin : depthValue;
				depthMax = depthMax > depthValue ? depthMax : depthValue;
			}
		}

		// should really stop hard coding the viewport
		for(int32_t y = 0; y < SIZE_Y; ++y)
		{
			for(int32_t x = 0; x < SIZE_X; ++x)
			{
				uint8_t pixel[4];
				float depthValue;
				depth.ReadPixel(IPoint2D{ x, y }, &depthValue);

				depthValue = 1 - ((depthValue - depthMin) / (depthMax - depthMin));

				pixel[0] = static_cast<uint8_t>(depthValue * 255);
				pixel[1] = static_cast<uint8_t>(depthValue * 255);
				pixel[2] = static_cast<uint8_t>(depthValue * 255);
				pixel[3] = static_cast<uint8_t>(depthValue * 255);

				//color.ReadPixel(IPoint2D{ x, y }, pixel);
				dis.SetPixel(pixel[0], pixel[1], pixel[1], x, y);
			}
		}

		dis.EndRender();

		dis.CopyToBackbuffer();

		dis.Persent();

		// super duper precise time measurement
		time += 0.001f;
		// what i said percise not accurate
	}
}