#pragma once

#include <stdint.h>

// i should drop my math lib here
struct IPoint2D
{
	using TYPE = int32_t;
	union
	{
		struct
		{
			TYPE x;
			TYPE y;
		};
		TYPE v[2];
	};
};

struct Point4D
{
	using TYPE = float;
	union
	{
		struct
		{
			TYPE x;
			TYPE y;
			TYPE z;
			TYPE w;
		};
		TYPE v[4];
	};
};


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

constexpr float lerp(float a, float b, float t)
{
	return a * (1 - t) + b * t;

}

constexpr int32_t ftoi(float in)
{
	return int32_t(in + 0.5f);
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
