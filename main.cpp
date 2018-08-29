
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "DXApp.h"
#include <DirectXColors.h>
#include "Cube.h"
#include <fstream>
#include <vector>


class TestApp : public DXApp
{
public:
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool init() override;
	void update(float delta) override;
	void render(float delta) override;

	const Cube cube;
	ID3D11Buffer * vertex_buffer = nullptr;
	ID3D11Buffer * index_buffer = nullptr;

	ID3D11VertexShader * vertex_shader = nullptr;
	ID3D11PixelShader * pixel_shader = nullptr;

	ID3D11InputLayout * input_layout = nullptr;
};

TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	app_title = "My Awesome DirectX App";
}

TestApp::~TestApp()
{
	Memory::SafeRelease(vertex_buffer);
	Memory::SafeRelease(index_buffer);

	Memory::SafeRelease(vertex_shader);
	Memory::SafeRelease(pixel_shader);

	Memory::SafeRelease(input_layout);
}

bool TestApp::init()
{
	bool result = DXApp::init();

	if (!result)
		return false;

	// create vertex buffer desc
	auto vertex_buffer_desc = CD3D11_BUFFER_DESC(
		sizeof(Vertex) * 8,
		D3D11_BIND_VERTEX_BUFFER
	);

	// create vertex buffer
	D3D11_SUBRESOURCE_DATA vertex_data = { };
	vertex_data.pSysMem = cube.vertices;
	device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);

	// create index buffer desc
	auto index_buffer_desc = CD3D11_BUFFER_DESC(
		sizeof(cube.indexes),
		D3D11_BIND_INDEX_BUFFER
	);

	// create index buffer
	D3D11_SUBRESOURCE_DATA index_data = { };
	index_data.pSysMem = cube.indexes;
	device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);

	// create vertex shader
	std::ifstream vsf("dvs.cso", std::ios::binary);
	std::vector<char> vs_data =
	{
		std::istreambuf_iterator<char>(vsf),
		std::istreambuf_iterator<char>()
	};
	device->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &vertex_shader);

	// create pixel shader
	std::ifstream psf("dps.cso", std::ios::binary);
	std::vector<char> ps_data =
	{
		std::istreambuf_iterator<char>(psf),
		std::istreambuf_iterator<char>()
	};
	device->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &pixel_shader);

	// create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	device->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &input_layout);

	return result;
}

void TestApp::update(float delta)
{
}

void TestApp::render(float delta)
{
	float color[] = { 0.2F, 0.2F, 0.2F, 1.0F };
	device_context->ClearRenderTargetView(render_target_view, color);

	// bind input layout
	device_context->IASetInputLayout(input_layout);

	// bind shaders
	device_context->VSSetShader(vertex_shader, nullptr, 0);
	device_context->PSSetShader(pixel_shader, nullptr, 0);

	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// bind index buffer
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// draw
	device_context->DrawIndexed(ARRAYSIZE(cube.vertices), 0, 3);

	swap_chain->Present(0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp ta(hInstance);

	if (!ta.init())
		return 1;

	return ta.run();
}
