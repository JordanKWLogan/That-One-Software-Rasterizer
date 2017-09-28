#include "Texture.h"

#include <cstring>

// future table of horrors
uint32_t DATA_FORMAT_SIZE[DATA_FORMAT::COUNT] =
{
	4, //RGBA8,
	4, //D32,
};


Texture::Texture(void)
	: m_Desc{}
	, m_Data(nullptr)
{
}

void Texture::CreateTexture(TextureDesc desc)
{
	// we dont do any error checking cuse we cool like dat /s
	uint32_t size = desc.Width * desc.Height * DATA_FORMAT_SIZE[desc.Format];

	m_Data = new uint8_t[size];
	m_Desc = desc;
}

void Texture::DeleateTexture(void)
{
	delete[] m_Data;
}

void Texture::SetPixel(IPoint2D const & point, void const* data)
{
	const uint32_t pitch = m_Desc.Width * DATA_FORMAT_SIZE[m_Desc.Format];
	memcpy(m_Data + pitch * point.y + point.x * DATA_FORMAT_SIZE[m_Desc.Format], data, DATA_FORMAT_SIZE[m_Desc.Format]);
}

void Texture::ReadPixel(IPoint2D const & point, void * data) const
{
	const uint32_t pitch = m_Desc.Width * DATA_FORMAT_SIZE[m_Desc.Format];
	memcpy(data, m_Data + pitch * point.y + point.x * DATA_FORMAT_SIZE[m_Desc.Format], DATA_FORMAT_SIZE[m_Desc.Format]);
}

void Texture::Clear(void const * data)
{
	const uint32_t pitch = m_Desc.Width * DATA_FORMAT_SIZE[m_Desc.Format];

	// later we will have lots of cool optimizations here
	// for now just cry
	for(uint32_t y = 0; y < m_Desc.Height; ++y)
	{
		for(uint32_t x = 0; x < m_Desc.Width; ++x)
		{
			memcpy(m_Data + pitch * y + x * DATA_FORMAT_SIZE[m_Desc.Format], data, DATA_FORMAT_SIZE[m_Desc.Format]);
		}

	}
}
