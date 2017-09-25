#pragma once

#include <stdint.h>


class Window
{
	public:

	void Initialize(uint32_t width, uint32_t height);
	void Update(void);

	void SetHInstance(void* hInstance);

	void* GetHWND();

	void ActivateWindow();

	private:
	void* m_HInstance;
	void* m_WindowHandle;

	uint32_t m_Width;
	uint32_t m_Height;
};
