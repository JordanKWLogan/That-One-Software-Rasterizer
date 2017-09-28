#pragma once

#include "Point.h"

// old hacky way of doing enums since enum classes are annoying with how strict they are. Its cuase they have "CLASS"
namespace DATA_FORMAT
{
	enum TYPE : uint32_t
	{
		RGBA8,
		D32,
		COUNT
	};
}

struct TextureDesc
{
	uint32_t Width;
	uint32_t Height;
	//uint32_t Depth; //someday later
	// TextureType Type; // uhh yeah we will do other types. unless i forget about this project or ...
	// TextureLayout Layout; // we walk the stright and narrow linear line
	DATA_FORMAT::TYPE Format;
};

class Texture
{
	public:
	Texture(void);


	void CreateTexture(TextureDesc desc);
	void DeleateTexture(void);

	// will need to change this to account for 3D/mip/arrays
	// Hoping im smart enough to always pass in the corrently sized data in the right format :/
	void SetPixel(IPoint2D const& point, void const* data);

	// same thing here. dont be dumb
	void ReadPixel(IPoint2D const& point, void* data) const;

	// same thing here. dont be dumb
	void Clear(void const* data);

	private:

	TextureDesc m_Desc;
	uint8_t* m_Data;
};
