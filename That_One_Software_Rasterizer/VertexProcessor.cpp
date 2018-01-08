
#include "VertexProcessor.h"

#include <cstring>

uint32_t VertexProcessor::ProcessDataSteam(VertexShader shader, uint32_t vertices, uint32_t vertexOffset, VertexData const& data)
{
	m_LVSPos = 0; // later we wont do this.... I hope
	m_LVPPos = 0; // should change the Very confussing names

	uint8_t scratchData[MAX_VERTEX_SIZE];

	uint32_t count = 0;
	for(uint32_t i = 0; i < vertices; i += 3)
	{
		// lets assume triangles. that wont hurt us later ;)
		for(uint32_t j = 0; j < 3; ++j)
		{
			for(uint32_t k = 0; k < data.attributeCount; k++)
			{
				Attribute const& att = data.attributes[k];

				const uint32_t offset = att.stride * (i + j + vertexOffset);
				const uint8_t* stream = data.streams[att.stream];

				memcpy(scratchData + att.offset, stream + offset, att.size);
			}

			uint8_t* outData = m_LocalVertexStore + m_LVSPos + j;

			shader(scratchData, outData, m_LocalVertexPositions[m_LVPPos + j]);


			// missing a very importent thing here
			// m_LVSPos += x;
		}


		// dear god this needs help in here
		uint32_t n = Clip(m_LVPPos);

		for(uint32_t j = 0; j < n; ++j)
		{
			const float w = m_LocalVertexPositions[m_LVPPos].w;
			// lets get to NDC (Not dat coord)
			m_LocalVertexPositions[m_LVPPos].x /= w;
			m_LocalVertexPositions[m_LVPPos].y /= w;
			m_LocalVertexPositions[m_LVPPos].z /= w;
			//m_LocalVertexPositions[m_LVPPos].w /= m_LocalVertexPositions[m_LVPPos].w;

			// hacks be here
			m_LocalVertexPositions[m_LVPPos].y = -m_LocalVertexPositions[m_LVPPos].y;
			++m_LVPPos;
		}

		count += n;
	}



	return count;
}

uint32_t VertexProcessor::Clip(uint32_t index)
{
	Point4D* localPoints = m_LocalVertexPositions + index;

	Point4D ringBuffer[64]; // need a better ring... not one made of bandades
	uint32_t ringOffset = 0;
	uint32_t ringTailOffset = 3; // 3 is not a magic number... right?

	// preload ring buffer
	ringBuffer[0] = localPoints[0];
	ringBuffer[1] = localPoints[1];
	ringBuffer[2] = localPoints[2];

	uint32_t triCount = 1;
	for(uint32_t p = 0; p < 2; ++p)
	{
		for(int32_t s = -1; s < 2; s += 2)
		{
			uint32_t oldRingOffset = ringTailOffset;
			const uint32_t oldTriCount = triCount;
			for(uint32_t tri = 0; tri < oldTriCount; ++tri)
			{
				// pro log
				uint32_t count = 0;

				Point4D points[4];
				points[0] = ringBuffer[ringOffset + tri * 3 + 0];
				points[1] = ringBuffer[ringOffset + tri * 3 + 1];
				points[2] = ringBuffer[ringOffset + tri * 3 + 2];
				points[3] = ringBuffer[ringOffset + tri * 3 + 0];

				Point4D *const currentRing = ringBuffer + ringTailOffset;

				for(uint32_t i = 0; i < 3; ++i)
				{
					const float x0 = s * points[i].v[p];
					const float x1 = s * points[i + 1].v[p];
					const float w0 = points[i].w;
					const float w1 = points[i + 1].w;

					// both points are inside
					if(x0 < w0 && x1 < w1)
					{
						currentRing[count] = points[i + 1];
						++count;
					}
					// point a is inside, point b is outside
					else if(x0 < w0 && x1 >= w1)
					{
						const float scale = (x0 - w0) / (x0 + w1 - (x1 + w0));
						/*
						x = w
						x0 + (x1 - x0) * t = w0 + (w1 - w0) * t
						x0 - w0 = (w1 - w0 - x1 + x0) * t
						(x0 - w0) / (w1 + x0 - w0 - x1) = t
						*/
						Point4D point;
						point.x = lerp(points[i].x, points[i + 1].x, scale);
						point.y = lerp(points[i].y, points[i + 1].y, scale);
						point.z = lerp(points[i].z, points[i + 1].z, scale);
						point.w = lerp(points[i].w, points[i + 1].w, scale);

						currentRing[count] = point;
						++count;
					}
					// point a is outside, point b is inside
					else if(x0 >= w0 && x1 < w1)
					{
						const float scale = (x0 - w0) / (x0 + w1 - (x1 + w0));

						Point4D point;
						point.x = lerp(points[i].x, points[i + 1].x, scale);
						point.y = lerp(points[i].y, points[i + 1].y, scale);
						point.z = lerp(points[i].z, points[i + 1].z, scale);
						point.w = lerp(points[i].w, points[i + 1].w, scale);

						currentRing[count] = point;
						++count;

						currentRing[count] = points[i + 1];
						++count;
					}
				}

				// epi log

				if(count == 4)
				{
					triCount++;

					// copy extras
					currentRing[4] = currentRing[0];
					currentRing[5] = currentRing[2];
					count = 6;
				}
				else if(count == 0)
				{
					triCount--;
				}

				ringTailOffset += count;
			}

			ringOffset = oldRingOffset;
		}
	}

	uint32_t count = 0;
	// copy out of ring
	for(uint32_t i = 0; i < triCount; ++i)
	{
		localPoints[0] = ringBuffer[ringOffset + i * 3 + 0];
		localPoints[1] = ringBuffer[ringOffset + i * 3 + 1];
		localPoints[2] = ringBuffer[ringOffset + i * 3 + 2];

		uint32_t wClip = ClipW(index);

		localPoints += wClip;
		index += wClip;
		count += wClip;
	}

	return count;
}

uint32_t VertexProcessor::ClipW(uint32_t index)
{
	Point4D *const localPoints = m_LocalVertexPositions + index;
	Point4D points[4];
	points[0] = localPoints[0];
	points[1] = localPoints[1];
	points[2] = localPoints[2];
	points[3] = localPoints[0];
	
	float z[4];
	z[0] = points[0].z;
	z[1] = points[1].z;
	z[2] = points[2].z;
	z[3] = points[0].z;

	float w[4];
	w[0] = z[0];// / points[0].w;
	w[1] = z[1];// / points[1].w;
	w[2] = z[2];// / points[2].w;
	w[3] = z[0];// / points[0].w;

	uint32_t count = 0;
	for(uint32_t i = 0; i < 3; ++i)
	{
		// both points are inside
		if(z[i] > 0 && z[i + 1] > 0)
		{
			localPoints[count] = points[i + 1];
			++count;
		}
		// point a is inside, point b is outside
		else if(z[i] > 0 && z[i + 1] <= 0)
		{
			const float scale = (0 - w[i]) / (w[i + 1] - w[i]);

			Point4D point;
			point.x = lerp(points[i].x, points[i + 1].x, scale);
			point.y = lerp(points[i].y, points[i + 1].y, scale);
			point.z = lerp(points[i].z, points[i + 1].z, scale);
			point.w = lerp(points[i].w, points[i + 1].w, scale);

			localPoints[count] = point;
			++count;

			//localPoints[count] = points[i + 1];
			//++count;
		}
		// point a is outside, point b is inside
		else if(z[i] <= 0 && z[i + 1] > 0)
		{
			const float scale = (0 - w[i]) / (w[i + 1] - w[i]);	
			
			Point4D point;
			point.x = lerp(points[i].x, points[i + 1].x, scale);
			point.y = lerp(points[i].y, points[i + 1].y, scale);
			point.z = lerp(points[i].z, points[i + 1].z, scale);
			point.w = lerp(points[i].w, points[i + 1].w, scale);

			localPoints[count] = point;
			++count;

			localPoints[count] = points[i + 1];
			++count;
		}
	}

	if(count == 4)
	{
		// copy extras
		// need better cliping algo
		localPoints[4] = localPoints[0];
		localPoints[5] = localPoints[2];
		count = 6;
	}

	return count;
}

