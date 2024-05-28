#pragma once

#include "raylib.h"

union R3DE_Matrix
{
	Matrix mat;

	struct
	{
		Vector3 right;
		float right_w;

		Vector3 up;
		float up_w;

		Vector3 forward;
		float forward_w;

		Vector3 position;
		float position_w;
	} vec;

	float data[4][4];
	float raw[16];
};