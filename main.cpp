
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "DXApp.h"
#include <DirectXColors.h>


class TestApp : public DXApp
{
public:
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool init() override;
	void update(float delta) override;
	void render(float delta) override;
};

TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	app_title = "My Awesome DirectX App";
}

TestApp::~TestApp()
{
}

bool TestApp::init()
{
	return DXApp::init();
}

void TestApp::update(float delta)
{
}

void TestApp::render(float delta)
{
	float color[4] = { 0.8F, 0.0F, 0.2F, 1.0F };

	device_context->ClearRenderTargetView(render_target_view, color);
	swap_chain->Present(0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp ta(hInstance);

	if (!ta.init())
		return 1;

	return ta.run();
}
