#pragma once

#include "Engine.h"

class WPosMPos;

using namespace gce;

class UtilsPathfinding
{
public:
	static bool Normalize(gce::Vector3f32& vector);
	static float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2);
	static float GetDistance(Vector3f32 pos1, Vector3f32 pos2);
	static bool IsInside(WPosMPos shapePos, gce::Vector3f32 pos);
};

