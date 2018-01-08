#pragma once

#include "Point.h"
#include "Texture.h"

class Rasterizer
{
	public:

	void ProcessTriangle(Point4D const& v0, Point4D const& v1, Point4D const& v2);
	void RenderTriangle(Point4D const& v0, Point4D const& v1, Point4D const& v2);



	void RenderPixel(IPoint2D const& point, float z);

	void SetViewPort(IPoint2D const& point);

	// TODO BETTER CODE
	Texture* m_DepthTexture;
	Texture* m_ColorTexture;;

	// this is very temp
	// meaning most likly to stay forever
	uint8_t m_VERYTEMP_Color[4];
	private:
	

	IPoint2D m_Viewport;
	IPoint2D m_ViewportMinus1;
};