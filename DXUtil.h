#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace Memory
{
	template <class T>
	void SafeDelete(T & t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T>
	void SafeDeleteArray(T & t)
	{
		if (t)
		{
			delete [] t;
			t = nullptr;
		}
	}

	template <class T>
	void SafeRelease(T & t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}
