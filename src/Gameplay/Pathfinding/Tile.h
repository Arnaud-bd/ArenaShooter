#pragma once

#include "Engine.h"
#include "WPosMPos.h"

using namespace gce;

class Entity;

class Tile
{
private:
	bool mIsTempLock = false;
	Geometry* mShape = nullptr;

public:
	WPosMPos mPos;

	bool mIsObstacle = false;
	bool mIsLock = false;


	float mTimer = 4.f;
	float mTimerProgress = 4.f;

	Entity* mEntity = nullptr;

	bool mToDraw = true;

	void OnInitialize();
	void OnUpdate();
	void SetTempLock(bool isTempLock, Entity* newEntity = nullptr);
	bool IsTempLock() { return mIsTempLock; }
	void SetColor(gce::Color color);
	gce::Vector3f32 GetPosition() const;
	Geometry* GetShape() { return mShape; }
	void SetPosition(float x, float y, float z);
	void SetToDraw(bool toDraw) { mToDraw = toDraw; }
	bool IsToDraw() { return mToDraw; }
};

