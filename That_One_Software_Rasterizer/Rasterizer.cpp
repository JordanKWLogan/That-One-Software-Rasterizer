
#include "Rasterizer.h"

#include "Display.h" // for now


// help funcs
constexpr int32_t min(int32_t a, int32_t b)
{
	return (a < b) ? a : b;
}

constexpr int32_t max(int32_t a, int32_t b)
{
	return (a > b) ? a : b;
}

constexpr IPoint2D min(IPoint2D a, IPoint2D b)
{
	return IPoint2D{ min(a.x, b.x), min(a.y, b.y) };
}

constexpr IPoint2D max(IPoint2D a, IPoint2D b)
{
	return IPoint2D{ max(a.x, b.x), max(a.y, b.y) };
}

constexpr int32_t min3(int32_t a, int32_t b, int32_t c)
{
	return min(a, min(b, c));
}

constexpr int32_t max3(int32_t a, int32_t b, int32_t c)
{
	return max(a, max(b, c));
}

constexpr IPoint2D min3(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c)
{
	return IPoint2D{ min3(a.x, b.x, c.x), min3(a.y, b.y, c.y) };
}

constexpr IPoint2D max3(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c)
{
	return IPoint2D{ max3(a.x, b.x, c.x), max3(a.y, b.y, c.y) };
}

constexpr int32_t ftoi(float in)
{
	return int32_t(in);
}

constexpr float itof(int32_t in)
{
	return float(in);
}

// the fun orient function
// https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
constexpr int32_t orient2d(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// fun idea. constexpr renderer

// painful consts
const IPoint2D IPOINT2D_ZERO = { 0, 0 }; // yes this is a hack
const IPoint2D SCREEN_SIZE = { 200, 100 }; // yes this is a hack


void Rasterizer::RenderTrinagle(Point4D const& v0, Point4D const& v1, Point4D const& v2)
{
	// should center the pixels since we dont like alt left rendering (top left)
	const IPoint2D iv0 = { ftoi(v0.x), ftoi(v0.y) };
	const IPoint2D iv1 = { ftoi(v1.x), ftoi(v1.y) };
	const IPoint2D iv2 = { ftoi(v2.x), ftoi(v2.y) };

	// [witty comment about how the area is something]
	int32_t area = orient2d(iv0, iv1, iv2);
	float oneOverArea = 1.0f / itof(area);

	// edge set up
	IPoint2D minPoint = min3(iv0, iv1, iv2);
	IPoint2D maxPoint = max3(iv0, iv1, iv2); // DJ MAX POINT

	// fine we will clip these points with the view rect
	minPoint = max(minPoint, IPOINT2D_ZERO);
	maxPoint = min(maxPoint, SCREEN_SIZE);

	// loop on Y
	for(int32_t y = minPoint.y; y <= maxPoint.y; ++y)
	{
		// loop on x
		for(int32_t x = minPoint.x; x <= maxPoint.x; ++x)
		{
			const IPoint2D p = { x, y };
			// barycentric coordinates... illuminati confirmed
			int32_t w0 = orient2d(iv1, iv2, p);
			int32_t w1 = orient2d(iv2, iv0, p);
			int32_t w2 = orient2d(iv0, iv1, p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				// SLOW MODE GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
				float z = w0 * v0.w * oneOverArea
						+ w1 * v1.w * oneOverArea
						+ w2 * v2.w * oneOverArea;
				RenderPixel(p, z);
			}
		}
	}
}


// laster will take a mask
void Rasterizer::RenderPixel(IPoint2D const& point, float z)
{
	// should do something with this. like i dont know... use it to check if this triangle is in front of the current value
	// yeah that sounds like a good idea
	(void)z; // shut up mr compiler
	const uint8_t color[4] = { 255,255,255,0 };
	m_ColorTexture->SetPixel(point, color);
}