#pragma once

#include "Enemy.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Spider2, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:
	void Attack() override;

	void OnStartRush();
	void OnUpdateRush();

	Vector3f32 mRushDirection;
	Vector3f32 mBeginPos;
	float mDistanceRush = 25.f;
	float mRushSpeed = 2.f;

	float mCooldownRushTime = 2.f;
	float mCooldownRushProgress = 2.f;

	bool mIsInRush = false;

public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);

	END_SCRIPT