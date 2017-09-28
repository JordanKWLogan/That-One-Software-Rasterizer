#pragma once

#include "Point.h"
#include "Texture.h"

class Rasterizer
{
	public:

	void RenderTrinagle(Point4D const& v0, Point4D const& v1, Point4D const& v2);


	void RenderPixel(IPoint2D const& point, float z);

	// TODO BETTER CODE
	Texture* m_DepthTexture;
	Texture* m_ColorTexture;
	private:
	
};