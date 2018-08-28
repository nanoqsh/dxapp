#pragma once

#include <Windows.h>
#include <string>
#include "DXUtil.h"


class DXApp
{
protected:
	HWND handle_app_window;
	HINSTANCE handle_app_instance;
	UINT client_width;
	UINT client_height;
	std::string app_title;
	DWORD window_style;

	bool init_window();

	// DirectX attributes
	ID3D11Device * device;
	ID3D11DeviceContext * device_context;
	IDXGISwapChain * swap_chain;
	ID3D11RenderTargetView * render_target_view;
	D3D_DRIVER_TYPE driver_type;
	D3D_FEATURE_LEVEL feature_level;
	D3D11_VIEWPORT viewport;

	bool init_direct();

public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

	int run(); // main loop
	virtual bool init();
	virtual void update(float delta) = 0;
	virtual void render(float delta) = 0;
	virtual LRESULT message_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);
};

