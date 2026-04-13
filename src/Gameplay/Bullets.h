#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(Bullets, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

protected:

	gce::Vector3f32 mDirection;

	float mBulletSpeed;
	float mLifeTime;
	int mDamage;
	bool mToDestroy = false;


public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);
	int GetDamage() { return mDamage; }
	void SetDamage(int damage) { mDamage = damage; }

	END_SCRIPT


