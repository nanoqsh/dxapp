#include "DXApp.h"


namespace
{
	DXApp * gdxapp = nullptr;
}

LRESULT CALLBACK global_message_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	return gdxapp ?
		gdxapp->message_proc(hwnd, message, wp, lp) :
		DefWindowProc(hwnd, message, wp, lp);
}

DXApp::DXApp(HINSTANCE hInstance)
{
	handle_app_instance = hInstance;
	handle_app_window = nullptr;
	client_width = 800;
	client_height = 600;
	app_title = "DIRECTX11 APP";
	window_style = WS_OVERLAPPEDWINDOW;
	gdxapp = this;

	device = nullptr;
	device_context = nullptr;
	render_target_view = nullptr;
	swap_chain = nullptr;
}

DXApp::~DXApp()
{
	// cleanup direct
	if (device_context)
		device_context->ClearState();

	Memory::SafeRelease(render_target_view);
	Memory::SafeRelease(swap_chain);
	Memory::SafeRelease(device_context);
	Memory::SafeRelease(device);
}

int DXApp::run()
{
	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			update(0.0f);
			render(0.0f);
		}
	}

	return static_cast<int>(msg.wParam);
}

bool DXApp::init()
{
	return init_window() && init_direct();
}

bool DXApp::init_direct()
{
	UINT create_device_flags = 0;

	// You need Graphics Tools for Windows 10
 	//#ifdef _DEBUG
	//create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif // _DEBUG

	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { };

	swap_chain_desc.BufferCount = 1; // Two buffers
	swap_chain_desc.BufferDesc.Width = client_width;
	swap_chain_desc.BufferDesc.Height = client_height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = handle_app_window;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result;
	for (int i = 0; i < ARRAYSIZE(driver_types); ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driver_types[i],
			nullptr,
			create_device_flags,
			feature_levels,
			ARRAYSIZE(feature_levels),
			D3D11_SDK_VERSION,
			&swap_chain_desc,
			&swap_chain,
			&device,
			&feature_level,
			&device_context
		);

		if (SUCCEEDED(result))
		{
			driver_type = driver_types[i];
			break;
		}
	}

	if (FAILED(result))
	{
		OutputDebugString("FAILED TO CREATE DEVICE AND SWAP CHAIN!\n");
		return false;
	}

	// create render target view
	ID3D11Texture2D * back_buffer_texture = nullptr;
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&back_buffer_texture));
	device->CreateRenderTargetView(back_buffer_texture, nullptr, &render_target_view);

	// bind render target view
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);

	// viewport creation
	viewport.Width = static_cast<float>(client_width);
	viewport.Height = static_cast<float>(client_height);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// bind viewport
	device_context->RSSetViewports(1, &viewport);

	return true;
}

LRESULT DXApp::message_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wp, lp);
	}
}

bool DXApp::init_window()
{
	WNDCLASSEX wcex = { };
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = handle_app_instance;
	wcex.lpfnWndProc = global_message_proc;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "DXAPPWNDCLASS";

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("FAILED TO CREATE WINDOW CLASS!\n");
		return false;
	}

	RECT r = { 0, 0, static_cast<LONG>(client_width), static_cast<LONG>(client_height) };
	AdjustWindowRect(&r, window_style, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;

	handle_app_window = CreateWindow(
		"DXAPPWNDCLASS",
		app_title.c_str(),
		window_style,
		GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
		width,
		height,
		nullptr,
		nullptr,
		handle_app_instance,
		nullptr
	);

	if (!handle_app_window)
	{
		OutputDebugString("FAILED TO CREATE WINDOW!\n");
		return false;
	}

	ShowWindow(handle_app_window, SW_SHOW);

	return true;
}
