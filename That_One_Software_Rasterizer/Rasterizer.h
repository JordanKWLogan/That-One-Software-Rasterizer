#pragma once

#include <stdint.h>


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

class Rasterizer
{
	public:

	void RenderTrinagle(IPoint2D const& a, IPoint2D const& b, IPoint2D const& c);


	void RenderPixel(IPoint2D const& point);
	private:
	
};