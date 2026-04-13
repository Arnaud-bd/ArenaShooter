#pragma once

#include "Engine.h"
#include "Life.h"
#include "Render.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Entity, Life, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

struct Target
{
	gce::Vector3f32 position;
	float distance;
	bool isSet;
};

private:
	Geometry* m_pGeo = nullptr;

	gce::Vector3f32 mDirection;
	float mSpeed = 0.f;
	Target mTarget;

public:
	void Start();
	void Update();

	void SetDirection(float x, float y, float z, float speed = -1.f);

	bool GoToDirection(int x, int y, float z, float speed = -1.f);
	bool GoToPosition(int x, int y, float z, float speed = -1.f);
	bool GoToPosition(Vector3f32 pos, float speed = -1.f);

	void UnsetTarget();

	GameObject* GetOwner() { return m_pOwner; }
	void SetSpeed(float newSpeed) { mSpeed = newSpeed; }

	END_SCRIPT