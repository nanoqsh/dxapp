#pragma once
#include <d3d.h>


struct Vertex
{
	float x, y, z;
	float r, g, b;
};

class Cube
{
public:
	const Vertex vertices[8] =
	{
		{ 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f }
	};

	const UINT indexes[3 * 12] =
	{
		0, 1, 3,
		1, 2, 3,
		4, 0, 7,
		7, 0, 3,
		2, 7, 3,
		2, 6, 7,

		0, 4, 1,
		1, 4, 5,
		5, 4, 7,
		5, 7, 6,
		1, 5, 6,
		1, 6, 2
	};

	Cube() = default;
	virtual ~Cube() = default;
};

