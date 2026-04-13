#pragma once

#include "Enemy.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Boss, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:
	void Attack() override;

	void OnStartSwing();
	void OnUpdateSwing();
	void OnEndSwing();

	void OnStartThrow();
	void OnUpdateThrow();
	void OnEndThrow();

	void CreateRock();

	void SpecialAttack();
	bool mHasHead = true;

	GameObject* mBody = nullptr;
	GameObject* mNeck = nullptr;
	GameObject* mHead = nullptr;

	//Swing
	float mCurretnRotate = 0.f;
	bool mHasDealDamageSwing = false;

	//Throw
	float mCurrentRotateThrow = 0.f;
	int mFactor = 1;
	GameObject* mRock = nullptr;

public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);

	END_SCRIPT