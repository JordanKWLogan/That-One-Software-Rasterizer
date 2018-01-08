
#include "Rasterizer.h"

#include "Display.h" // for now

#include <cfloat>


// fun idea. constexpr renderer


// painful consts
const IPoint2D IPOINT2D_ZERO = { 0, 0 }; // yes this is a hack

void Rasterizer::RenderTriangle(Point4D const& v0, Point4D const& v1, Point4D const& v2)
{
	// should center the pixels since we dont like alt left rendering (top left)
	const IPoint2D iv0 = { ftoi(v0.x * m_Viewport.x), ftoi(v0.y * m_Viewport.y) };
	const IPoint2D iv1 = { ftoi(v1.x * m_Viewport.x), ftoi(v1.y * m_Viewport.y) };
	const IPoint2D iv2 = { ftoi(v2.x * m_Viewport.x), ftoi(v2.y * m_Viewport.y) };

	//if(
	//	(iv0.x < IPOINT2D_ZERO.x &&
	//	 iv1.x < IPOINT2D_ZERO.x &&
	//	 iv2.x < IPOINT2D_ZERO.x)
	//	||
	//	(iv0.x > m_ViewportMinus1.x &&
	//	 iv1.x > m_ViewportMinus1.x &&
	//	 iv2.x > m_ViewportMinus1.x)
	//	||
	//	(iv0.y < IPOINT2D_ZERO.y &&
	//	 iv1.y < IPOINT2D_ZERO.y &&
	//	 iv2.y < IPOINT2D_ZERO.y)
	//	||
	//	(iv0.y > m_ViewportMinus1.y &&
	//	 iv1.y > m_ViewportMinus1.y &&
	//	 iv2.y > m_ViewportMinus1.y)
	//	)
	//{
	//	return;
	//}

	// [witty comment about how the area is something]
	int32_t area = orient2d(iv0, iv1, iv2);
	float oneOverArea = 1.0f / itof(area);

	// get rid of the pesky zero area cases
	if(area <= 0)
		return;

	// edge set up
	IPoint2D minPoint = min3(iv0, iv1, iv2);
	IPoint2D maxPoint = max3(iv0, iv1, iv2); // DJ MAX POINT

	// fine we will clip these points with the view rect
	minPoint = max(minPoint, IPOINT2D_ZERO);
	maxPoint = min(maxPoint, m_ViewportMinus1);

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
				float z = w0 * v0.z * oneOverArea
						+ w1 * v1.z * oneOverArea
						+ w2 * v2.z * oneOverArea;
				RenderPixel(p, z);
				if(_isnan(z)) __debugbreak();
			}
		}
	}
}


// laster will take a mask
void Rasterizer::RenderPixel(IPoint2D const& point, float z)
{
	if(z < 0 || z > 1)
		return;

	float pixelDepth;
	m_DepthTexture->ReadPixel(point, &pixelDepth);

	// hey look we use the depth. an't that nice
	if(pixelDepth < z)
		return;

	m_ColorTexture->SetPixel(point, m_VERYTEMP_Color);

	// dont forget to set it. :D
	m_DepthTexture->SetPixel(point, &z);
}

void Rasterizer::SetViewPort(IPoint2D const& point)
{
	m_Viewport = point;
	m_ViewportMinus1 = IPoint2D{ point.x - 1, point.y - 1 };
}