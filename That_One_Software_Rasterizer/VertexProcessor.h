#pragma once

#include "Point.h"

constexpr static int32_t MAX_ATTRIBUTES = 32;
constexpr static int32_t MAX_STREAMS = 8;
constexpr static int32_t MAX_VERTEX_SIZE = 256;
constexpr static int32_t LOCAL_VERTEX_STORE_SIZE = 1 * 1024; // 1Mb

using VertexShader = void (*)(uint8_t* inData, uint8_t* outData, Point4D& outPoint);


struct Attribute
{
	// source. from the SOURCE of life or i guess data. Same thing
	uint32_t stream;
	uint32_t stride;

	// where it goes
	uint32_t offset;
	uint32_t size;
};


struct VertexData
{
	uint32_t  attributeCount;
	Attribute attributes[MAX_ATTRIBUTES];

	uint8_t*  streams[MAX_STREAMS];
};

class VertexProcessor
{
	public:

	// at some point VertexData will be part of the state but not yet since it is still skeleton work here
	// in other news the skeleton wars are coming
	uint32_t ProcessDataSteam(VertexShader shader, uint32_t vertices, uint32_t vertexOffset, VertexData const& data);

	// This should not be public. I dont want people touching my privates but i keep leaving them out :P
	uint8_t m_LocalVertexStore[LOCAL_VERTEX_STORE_SIZE];
	uint32_t m_LVSPos;

	Point4D m_LocalVertexPositions[LOCAL_VERTEX_STORE_SIZE];
	uint32_t m_LVPPos;
	private:

};