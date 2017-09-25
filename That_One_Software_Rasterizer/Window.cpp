
#include "Window.h"

#define WINDOWS_LEAN_AND_MEAN
#define NO_MIN_MAX
#include <Windows.h>

static const wchar_t className[] = L"GameProjectWindowClass";
static const wchar_t windowName[] = L"That One Software Rasterizer";

static LRESULT CALLBACK WindowCallback(HWND hWin, UINT msg, WPARAM wParam, LPARAM lp);

void Window::Initialize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	// Figure out where the window is located.
	int screenX = ((GetSystemMetrics(SM_CXSCREEN) - m_Width) / 2);
	//#ifdef _DEBUG
	//  screenX += GetWidth() / 2;
	//#endif
	int screenY = (GetSystemMetrics(SM_CYSCREEN) - m_Height) / 2;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowCallback; // Indirection for replay
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = reinterpret_cast<HINSTANCE>(m_HInstance);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	wc.hIconSm = 0;
	wc.hIcon = 0;

	RegisterClassEx(&wc);

	m_WindowHandle =
		CreateWindow(className,
					 windowName,
					 windowStyle,
					 screenX, screenY,
					 m_Width,
					 m_Height,
					 GetDesktopWindow(),
					 NULL,
					 reinterpret_cast<HINSTANCE>(m_HInstance),
					 NULL);

	// Position the window in the middle of the screen and calculate the actual drawing window
	//// size not including borders or top menu bar.
	//GetClientRect(m_WindowHandle, &rcClient);
	//GetWindowRect(m_WindowHandle, &rcWind);
	//ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	//ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	//MoveWindow(m_WindowHandle, rcWind.left - ptDiff.x, rcWind.top - ptDiff.y, GetWidth() + ptDiff.x, GetHeight() + ptDiff.y, TRUE);
	//m_WidthDiff = ptDiff.x;
	//m_HeightDiff = ptDiff.y;


	////SetWindowPos(m_WindowHandle, HWND_TOP, 0, 0, 1920, 1080, 0);

	//gInputManager->Initialize();

	//m_Initialized = true;
}

void Window::SetHInstance(void* hInstance)
{
	m_HInstance = hInstance;
}

void Window::Update(void)
{
	MSG msg;

	while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void* Window::GetHWND()
{
	return m_WindowHandle;
}

void Window::ActivateWindow()
{
	ShowWindow(reinterpret_cast<HWND>(m_WindowHandle), SW_SHOWNA);
	UpdateWindow(reinterpret_cast<HWND>(m_WindowHandle));
	SetForegroundWindow(reinterpret_cast<HWND>(m_WindowHandle));
}

LRESULT CALLBACK WindowCallback(HWND hWin, UINT msg, WPARAM wParam, LPARAM lp)
{
	HDC dc;
	PAINTSTRUCT ps;

	switch(msg)
	{
		// when the rectangle is drawn
		case WM_PAINT:
			dc = BeginPaint(hWin, &ps);
			EndPaint(hWin, &ps);
			break;

			// When it's time for the window to go away
		case WM_DESTROY:
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

			// called any time the window is moved
		case WM_MOVE:
			// Invalidate the rect to force a redraw
			InvalidateRect(hWin, NULL, FALSE);
			break;

		case WM_ACTIVATE:
			break;

		case WM_ENTERSIZEMOVE:
			break;

		case WM_MDIMAXIMIZE:
			break;

		case WM_EXITSIZEMOVE:
			break;

		case WM_SIZE:
		{
		}
		break;

		case WM_MENUCHAR:
		{
			return MNC_CLOSE << 16;
			break;
		}

		default:
			return DefWindowProc(hWin, msg, wParam, lp);
	}
	return 0;
}