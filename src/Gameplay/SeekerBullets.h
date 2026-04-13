#pragma once

#include "Bullets.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(SeekerBullets, Bullets, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:

	float mSeekProcess;
	float mSeekTime;
	GameObject* m_pTarget = nullptr;

public:
	void Start();
	void Update();

	END_SCRIPT