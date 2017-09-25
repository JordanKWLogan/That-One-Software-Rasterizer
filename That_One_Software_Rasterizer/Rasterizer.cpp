
#include "Rasterizer.h"

#include "Display.h" // for now

// i know, i know. not great
extern Display* g_NotGreatDisplay;


// help funcs
constexpr int32_t min(int32_t a, int32_t b)
{
	return (a < b) ? a : b;
}

constexpr int32_t max(int32_t a, int32_t b)
{
	return (a > b) ? a : b;
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


// the fun orient function
// https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
constexpr int32_t orient2d(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// fun idea. constexpr renderer

// painful consts


void Rasterizer::RenderTrinagle(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c)
{
	// edge set up

	const IPoint2D minPoint = min3(a, b, c);
	const IPoint2D maxPoint = max3(a, b, c); // DJ MAX POINT

	// uhhh cliping may be a good idea
	// or we could keep with the current way
	// crash on out of bounds or memory corruption

	// loop on Y
	for(int32_t y = minPoint.y; y <= maxPoint.y; ++y)
	{
		// loop on x
		for(int32_t x = minPoint.x; x <= maxPoint.x; ++x)
		{
			const IPoint2D p = { x, y };
			// barycentric coordinates... illuminati confirmed
			int32_t w0 = orient2d(b, c, p);
			int32_t w1 = orient2d(c, a, p);
			int32_t w2 = orient2d(a, b, p);

			if(w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				RenderPixel(p);
			}
		}
	}
}


// laster will take a mask
void Rasterizer::RenderPixel(IPoint2D const& point)
{
	g_NotGreatDisplay->SetPixel(255, 255, 255, point.x, point.y);
}