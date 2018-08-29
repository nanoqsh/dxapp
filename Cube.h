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
		{ -0.5f, -0.5f, -0.5f,  0,  0,  0},
		{ -0.5f, -0.5f,  0.5f,  0,  0,  1},
		{ -0.5f,  0.5f, -0.5f,  0,  1,  0},
		{ -0.5f,  0.5f,  0.5f,  0,  1,  1},

		{  0.5f, -0.5f, -0.5f,  1,  0,  0},
		{  0.5f, -0.5f,  0.5f,  1,  0,  1},
		{  0.5f,  0.5f, -0.5f,  1,  1,  0},
		{  0.5f,  0.5f,  0.5f,  1,  1,  1}
	};

	const UINT indexes[3 * 6 * 2] =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5
	};

	Cube() = default;
	virtual ~Cube() = default;
};

