
#include "VertexProcessor.h"

#include <cstring>

uint32_t VertexProcessor::ProcessDataSteam(VertexShader shader, uint32_t vertices, VertexData const& data)
{
	m_LVSPos = 0; // later we wont do this.... I hope
	m_LVPPos = 0; // should change the Very confussing names

	uint8_t scratchData[MAX_VERTEX_SIZE];

	for(uint32_t i = 0; i < vertices; i++)
	{
		for(uint32_t j = 0; j < data.attributeCount; j++)
		{
			Attribute const& att = data.attributes[j];

			const uint32_t offset = att.stride * i;
			const uint8_t* stream = data.streams[att.stream];

			memcpy(scratchData + att.offset, stream + offset, att.size);
		}

		uint8_t* outData = m_LocalVertexStore + m_LVSPos;

		shader(scratchData, outData, m_LocalVertexPositions[m_LVPPos]);
		++m_LVPPos;

		// missing a very importent thing here
		// m_LVSPos += x;
	}

	return 0;
}