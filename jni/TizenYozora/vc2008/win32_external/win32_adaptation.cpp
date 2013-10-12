
#include "../../src/flat_board/target_dep.h"

////////////////////////////////////////////////////////////////////////////////
// window

#include <windows.h>
#include <stdio.h>

#define	WINDOW_CLASS "DojiClass"

namespace
{
	HWND g_h_wnd = 0;
	HDC g_h_dc  = 0;

	LRESULT CALLBACK WndProc(HWND g_h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		switch (message)
		{
	#if 1
			case WM_KEYDOWN:
			{
				if (w_param == 27)
				{
					PostQuitMessage(0);
					return 1;
				}
				break;
			}
	#endif
			case WM_SYSCOMMAND:
			{
				switch (w_param)
				{
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
					return 0;
				}
				break;
			}

			case WM_CLOSE:
				PostQuitMessage(0);
				return 1;

			default:
				break;
		}

		return DefWindowProc(g_h_wnd, message, w_param, l_param);
	}

	HWND CreateNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth)
	{
		HINSTANCE h_instance = GetModuleHandle(NULL);

		// Register windows class
		{
			WNDCLASS window_class;

			window_class.style         = CS_HREDRAW | CS_VREDRAW;
			window_class.lpfnWndProc   = WndProc;
			window_class.cbClsExtra    = 0;
			window_class.cbWndExtra    = 0;
			window_class.hInstance     = h_instance;
			window_class.hIcon         = 0;
			window_class.hCursor       = 0;
			window_class.lpszMenuName  = 0;
			window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			window_class.lpszClassName = WINDOW_CLASS;

			ATOM registerClass         = RegisterClass(&window_class);

			if (!registerClass)
			{
				MessageBox(0, "Failed to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}

		struct Monitor
		{
			enum
			{
				MAX_CAPACITY = 2
			};

			int n_monitor;

			struct
			{
				int x, y, width, height;
			} res[MAX_CAPACITY];

			struct
			{
				int x, y, width, height;
			} work[MAX_CAPACITY];

			Monitor()
				: n_monitor(0)
			{
				struct cb
				{
					static BOOL CALLBACK EnumDisplayMonitors(HMONITOR h_monitor, HDC h_dc, LPRECT p_clip, LPARAM data)
					{
						Monitor* p_monitor = (Monitor*)data;

						if (p_monitor->n_monitor >= Monitor::MAX_CAPACITY)
							return FALSE;

						MONITORINFOEX monitor_info;
						monitor_info.cbSize = sizeof(monitor_info);
						GetMonitorInfo(h_monitor, &monitor_info);

						p_monitor->res[p_monitor->n_monitor].x      = monitor_info.rcMonitor.left;
						p_monitor->res[p_monitor->n_monitor].y      = monitor_info.rcMonitor.top;
						p_monitor->res[p_monitor->n_monitor].width  = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
						p_monitor->res[p_monitor->n_monitor].height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

						p_monitor->work[p_monitor->n_monitor].x      = monitor_info.rcWork.left;
						p_monitor->work[p_monitor->n_monitor].y      = monitor_info.rcWork.top;
						p_monitor->work[p_monitor->n_monitor].width  = monitor_info.rcWork.right - monitor_info.rcWork.left;
						p_monitor->work[p_monitor->n_monitor].height = monitor_info.rcWork.bottom - monitor_info.rcWork.top;

						++p_monitor->n_monitor;

						return TRUE;
					};
				};

				EnumDisplayMonitors(NULL, NULL, cb::EnumDisplayMonitors, (LPARAM)this);
			}
		} monitor_info;

		// Create window
		{
			::RECT	window_rect;
			SetRect(&window_rect, 0, 0, width, height);

			AdjustWindowRectEx(&window_rect, WS_CAPTION | WS_SYSMENU, false, 0);

			screen = (screen >= 0) ? screen : 0;
			screen = (screen < monitor_info.n_monitor) ? screen : (monitor_info.n_monitor - 1);

			int x = monitor_info.res[screen].x;
			int y = (monitor_info.res[screen].y + monitor_info.res[screen].height) - (window_rect.bottom - window_rect.top);

			y = (y >= 0) ? y : 0;

			g_h_wnd = CreateWindow
			(
				WINDOW_CLASS
				, "sm+gal"
				, WS_VISIBLE | WS_SYSMENU
				, x, y, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top
				, NULL, NULL
				, (HINSTANCE)h_instance, NULL
			);
		}

		g_h_dc = GetDC(g_h_wnd);

		return g_h_wnd;
	}

	void DestroyNativeWindow(void)
	{
		ReleaseDC(g_h_wnd, g_h_dc);
		g_h_dc = 0;

		DestroyWindow(g_h_wnd);
		g_h_wnd = 0;
	}

	bool ProcessMessage(unsigned long delay_time)
	{
		MSG msg;

		while (::PeekMessage(&msg, g_h_wnd, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return false;

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		::Sleep(delay_time);

		return true;
	}

}

HWND g_GetWindowHandle(void)
{
	return g_h_wnd;
}

////////////////////////////////////////////////////////////////////////////////
// main

#include "app_config.h"

#include <memory>

#if defined(_MSC_VER)
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#	define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	define ENABLE_MEMORY_MANAGER _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#	define ENABLE_MEMORY_MANAGER
#endif

namespace MAIN_APP
{
	struct BufferDesc
	{
		void* p_start_address;
		int   width;
		int   height;
		int   bytes_per_line;
		int   bits_per_pixel;
	};

	void init(const char* sz_id);
	void done(void);
	bool loop(const BufferDesc& buffer_desc);
}

void utilFilesToRes(void);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//utilFilesToRes();

	ENABLE_MEMORY_MANAGER
	//_CrtSetBreakAlloc(???);

	typedef   TARGET_PIXEL Pixel;
	const int BUFFER_W     = X_RESOLUTION;
	const int BUFFER_H     = Y_RESOLUTION;
	const int BUFFER_DEPTH = sizeof(Pixel) * 8;
	const int BUFFER_PITCH = BUFFER_W * sizeof(Pixel);

	CreateNativeWindow(1, X_VIEWPORT, Y_VIEWPORT, BUFFER_DEPTH);

	// instead of unique_ptr<>
	std::auto_ptr<Pixel> auto_buffer(new Pixel[BUFFER_W * BUFFER_H]);

	memset(auto_buffer.get(), 0, BUFFER_W * BUFFER_H * sizeof(Pixel));

	MAIN_APP::BufferDesc buffer_desc;
	{
		buffer_desc.p_start_address = auto_buffer.get();
		buffer_desc.width = BUFFER_W;
		buffer_desc.height = BUFFER_H;
		buffer_desc.bytes_per_line = BUFFER_PITCH;
		buffer_desc.bits_per_pixel = BUFFER_DEPTH;
	}

	BITMAPINFOHEADER bitmap_info;
	{
		memset(&bitmap_info, 0, sizeof(bitmap_info));

		bitmap_info.biSize = sizeof(bitmap_info);
		bitmap_info.biWidth = BUFFER_W;
		bitmap_info.biHeight = -BUFFER_H;
		bitmap_info.biPlanes = 1;
		bitmap_info.biBitCount = BUFFER_DEPTH;
		bitmap_info.biCompression = BI_RGB;
		bitmap_info.biSizeImage = BUFFER_PITCH * BUFFER_H;
		bitmap_info.biClrUsed = 0;
	}

	::SetStretchBltMode(g_h_dc, HALFTONE);

	MAIN_APP::init("yunjr");

	while (ProcessMessage(10))
	{
		if (!MAIN_APP::loop(buffer_desc))
			break;

		//OutputDebugString("*");

		if (X_RESOLUTION == X_VIEWPORT && Y_RESOLUTION == Y_VIEWPORT)
		{
			::SetDIBitsToDevice
			(
				g_h_dc
				, 0, 0, BUFFER_W, BUFFER_H
				, 0, 0, 0, BUFFER_H
				, buffer_desc.p_start_address, (BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS
			);
		}
		else
		{
			::StretchDIBits
			(
				g_h_dc
				, 0, 0, X_VIEWPORT, Y_VIEWPORT
				, 0, 0, BUFFER_W, BUFFER_H
				, buffer_desc.p_start_address, (BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS, SRCCOPY
			);

			/* faster method
				HBITMAP hBitmap = CreateBitmap(BUFFER_W, BUFFER_H, 1, BUFFER_DEPTH, buffer_desc.p_start_address);
				HDC hBitmapDC   = CreateCompatibleDC(g_h_dc);
				HGDIOBJ hOld    = SelectObject(hBitmapDC, (HGDIOBJ)hBitmap);

				SetStretchBltMode(g_h_dc, HALFTONE);

				// in while loop
				StretchBlt(g_h_dc, 0, 0, X_VIEWPORT, Y_VIEWPORT, hBitmapDC, 0, 0, BUFFER_W, BUFFER_H, SRCCOPY);

				SelectObject(hBitmapDC, hOld);
				DeleteObject(hBitmapDC);
			*/
		}
	}

	MAIN_APP::done();

	DestroyNativeWindow();

	return 0;
}
